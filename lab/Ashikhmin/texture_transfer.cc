#include "texture_transfer.h"
#include <cmath>
#include <random>
#include <limits>
#include "etf.h"

using namespace std;
using namespace glamorous;

TextureTransfer::TextureTransfer() : rd(), gen(rd()), dis(0, 1) {
    
}

void TextureTransfer::initialize(const cv::Mat &src, const cv::Mat &ref) {
    this->src = src.clone();
    this->ref = ref.clone();
    cv::Canny(ref, edge, 100, 300);
    // ETF etf_gen;
    // etf_gen.init_etf(src);
    // etf_gen.refine_etf(6, 3);
    // etf = etf_gen.get_etf();
    std::uniform_int_distribution<> dis_row(radius, ref.rows - 1 - radius);
    std::uniform_int_distribution<> dis_col(radius, ref.cols - 1 - radius);
    dst = cv::Mat::zeros(src.size(), CV_8UC3);
    g = vector<cv::Point2d>(dst.rows * dst.cols);
    for (int i = 0; i < dst.rows; ++i) {
        for (int j = 0; j < dst.cols; ++j) {
            cv::Point2d r(dis_col(gen), dis_row(gen));
            dst.at<cv::Vec3b>(i, j) = ref.at<cv::Vec3b>(r);
            g[i * dst.cols + j] = r;
        }
    }
    printf("init_done\n");
}

void TextureTransfer::scan() {
    for (int i = 0; i < dst.rows; ++i) {
        for (int j = 0; j < dst.cols; ++j) {
            vector<cv::Point2d> Q = build_Q(cv::Point2d(j, i));
            if (Q.empty()) {
                continue;
            }
            cv::Point2d r(j, i);
            cv::Point2d min_q(g[r.y * dst.cols + r.x]);
            float min_distance = get_distance(r, min_q);
            for (const auto &q : Q) {
                // if (edge.at<unsigned char>(q) > 127) {
                //     continue;
                // }
                float d = get_distance(r, q);
                if (d < min_distance) {
                    min_distance = d;
                    min_q = q;
                }
            }
            const cv::Vec3b &refp = ref.at<cv::Vec3b>(min_q);
            dst.at<cv::Vec3b>(r) = cv::Vec3b(refp[0], refp[1], refp[2]);
            g[r.y * dst.cols + r.x] = min_q;
        }
    }
}

cv::Mat TextureTransfer::get_dst() const {
    return dst.clone();
}

vector<cv::Point2d> TextureTransfer::build_Q(const cv::Point2d &r) {
    vector<cv::Point2d> Q;
    for (int dy = -radius; dy < 0; ++dy) {
        for (int dx = -radius; dx <= radius; ++dx) {
            try_add_Q(r, dx, dy, &Q);
        }
    }
    for (int dx = -radius; dx < 0; ++dx) {
        try_add_Q(r, dx, 0, &Q);
    }
    if (dis(gen) < p_rand) {
        std::uniform_int_distribution<> dis_row(radius, ref.rows - 1 - radius);
        std::uniform_int_distribution<> dis_col(radius, ref.cols - 1 - radius);
        cv::Point2d q(dis_col(gen), dis_row(gen));
        Q.push_back(q);
    }
    return Q;
}

float TextureTransfer::get_distance(const cv::Point2d &r, const cv::Point2d &q) const {
    return calc_dN(r, q) + wL * calc_dL(r, q) + 0.6 * calc_dI(r, q);
}

void TextureTransfer::try_add_Q(const cv::Point2d &r, int dx, int dy,
                                vector<cv::Point2d> *Q) {
    int y = r.y + dy;
    int x = r.x + dx;
    if (y < 0 || y >= dst.rows || x < 0 || x >= dst.cols) {
        return;
    }
    cv::Point2d rp = g[y * dst.rows + x];
    int qy = rp.y - dy;
    int qx = rp.x - dx;
    if (qy < 0 || qy >= ref.rows || qx < 0 || qx >= ref.cols) {
        return;
    }
    Q->push_back(cv::Point2d(qx, qy));
}

float TextureTransfer::calc_dN(const cv::Point2d &r, const cv::Point2d &q) const {
    float rb = 0;
    float rg = 0;
    float rr = 0;
    int rtot = 0;
    for (int dy = -radius; dy <= radius; ++dy) {
        for (int dx = -radius; dx <= radius; ++dx) {
            // if (dy == 0 && dx == 0) {
            //     continue;
            // }
            int ry = r.y + dy;
            int rx = r.x + dx;
            if (ry < 0 || ry >= src.rows || rx < 0 || rx >= src.cols) {
                continue;
            }
            const cv::Vec3b rbgr = src.at<cv::Vec3b>(ry, rx);
            rb += rbgr[0];
            rg += rbgr[1];
            rr += rbgr[2];
            ++rtot;
        }
    }
    rb /= rtot;
    rg /= rtot;
    rr /= rtot;
    
    float qb = 0;
    float qg = 0;
    float qr = 0;
    int qtot = 0;
    for (int dy = -radius; dy <= radius; ++dy) {
        for (int dx = -radius; dx <= radius; ++dx) {
            if (dy == 0 && dx == 0) {
                continue;
            }
            int qy = q.y + dy;
            int qx = q.x + dx;
            if (qy < 0 || qy >= ref.rows || qx < 0 || qx >= ref.cols) {
                continue;
            }
            const cv::Vec3b &qbgr = ref.at<cv::Vec3b>(qy, qx);
            qb += qbgr[0];
            qg += qbgr[1];
            qr += qbgr[2];
            ++qtot;
        }
    }
    qb /= qtot;
    qg /= qtot;
    qr /= qtot;
    
    float dN = sqrt((rb - qb) * (rb - qb) + (rg - qg) * (rg - qg) + (rr - qr) * (rr - qr));
    return dN;
}

float TextureTransfer::calc_dL(const cv::Point2d &r, const cv::Point2d &q) const {
    float d = 0;
    int dtot = 0;
    for (int dy = -radius; dy <= 0; ++dy) {
        for (int dx = -radius; dx <= radius; ++dx) {
            if (dy == 0 && dx == 0) {
                break;
            }
            int ry = r.y + dy;
            int rx = r.x + dx;
            float rb = 255;
            float rg = 255;
            float rr = 255;
            if (!(ry < 0 || ry >= src.rows || rx < 0 || rx >= src.cols)) {
                const cv::Vec3b rbgr = src.at<cv::Vec3b>(ry, rx);
                rb = rbgr[0];
                rg = rbgr[1];
                rr = rbgr[2];
            }
            else {
                continue;
            }
            int qy = q.y + dy;
            int qx = q.x + dx;
            float qb = 255;
            float qg = 255;
            float qr = 255;
            if (!(qy < 0 || qy >= ref.rows || qx < 0 || qx >= ref.cols)) {
                const cv::Vec3b &qbgr = ref.at<cv::Vec3b>(qy, qx);
                qb = qbgr[0];
                qg = qbgr[1];
                qr = qbgr[2];
            }
            else {
                continue;
            }
            d += sqrt((rb - qb) * (rb - qb) + (rg - qg) * (rg - qg) + (rr - qr) * (rr - qr));
            ++dtot;
        }
    }
    d /= dtot;
    return d;
}

float TextureTransfer::calc_dI(const cv::Point2d &r, const cv::Point2d &q) const {
/*    const cv::Vec3f &flow = etf.at<cv::Vec3f>(r);
    int x0 = r.x;
    int y0 = r.y;
    float dx = flow[1];
    float dy = flow[0];
    if (dx > 0) {
        dx = -dx;
        dy = -dy;
    }
    if (dx == 0 && dy == 0) {
        p = vector<cv::Point2d>(radius, r);
    }
    else if (dx == 0) {
        
    }
    int xa = p[0].y;
    int ya = p[0].x;
    int xb = p[1].y;
    int yb = p[1].x;
    printf("%d %d %f %f %d %d %d %d\n", x0, y0, dx, dy, xa, ya, xb, yb);
    float mrb = 0;
    float mrg = 0;
    float mrr = 0;
    for (int i = 1; i <= radius; ++i) {
        const cv::Point2d &px = p[i];
        int ry = px.y;
        int rx = px.x;
        float rb = 255;
        float rg = 255;
        float rr = 255;
        if (!(ry < 0 || ry >= dst.rows || rx < 0 || rx >= dst.cols)) {
            const cv::Vec3b &rbgr = dst.at<cv::Vec3b>(ry, rx);
            rb = rbgr[0];
            rg = rbgr[1];
            rr = rbgr[2];
        }
        mrb += rb;
        mrg += rg;
        mrr += rr;
    }
    mrb /= radius;
    mrg /= radius;
    mrr /= radius;
    const cv::Vec3b &qbgr = ref.at<cv::Vec3b>(q);
    float qb = qbgr[0];
    float qg = qbgr[1];
    float qr = qbgr[2];
    float dI = sqrt((mrb - qb) * (mrb - qb) + (mrg - qg) * (mrg - qg) + (mrr - qr) * (mrr - qr));
    return dI;*/
    return 0;
}
