#include "oilpaint_transfer.h"

namespace glamorous {
namespace oilpaint {

//====================자료형 선언====================//

typedef struct Point {
    int x;
    int y;
    struct Point * link;
} Point;

typedef struct Stroke { // stroke들이 들어감
    int r; // 원의 반지름이 들어감
    Point* pt; // x, y좌표를 저장함
               //int x, y; // x, y좌표를 저장함
    CvScalar g; // 색상값을 저장함
    struct Stroke *link; // 다음 값을 가르킴
} Stroke;

//=================================================//
 

//====================함수 사전 선언====================//

// 2.1 / 2.2 공통 사용 함수
Point* pushPoint(Point* head, int x, int y); // (x, y)좌표를 저장하는 함수 (연결 리스트형 스택 자료구조 활용)
void peekPoint(Point* head, int* x, int*y); // (x, y)좌표를 뱉는 함수 (연결 리스트형 스택 자료구조 활용)
                                            //void paint(IplImage* src, int *R, int mode);
void paintLayer(IplImage* canvas, IplImage* reference, int R, int mode); // 가장 기초가 되는 함수 (색칠할 x,y들을 구하는 등)
double difference(CvScalar a, CvScalar b); // 색상의 차이값을 구하는 함수
Stroke* makeStroke(Stroke* head, int r, Point* pt, CvScalar g); // 논문상의 S들을 저장하는 함수
Stroke* paintCanvas(Stroke* head, IplImage* canvas, int mode); // 논문상의 canvas 이미지에 색칠하는 함수

                                                               // 2.2 단독 사용 함수
Point* makeSplineStroke(int x0, int y0, int R, IplImage* refImage, IplImage* canvas); // 곡선모양의 선을 그리는 함수
int gradientMag(int x, int y, IplImage* refImage); // 사방의 색의 변화량이 없는지 확인하는 함수
void gradientDirection(int x, int y, double* gx, double* gy, IplImage* refImage); // 논문상의 (gx, gy)를 구하기 위한 함수 (색의 변화 방향)
double calculateColorAvg(CvScalar a); // 하나의 색의 평균을 구하는 함수(회색)

                                      //=================================================//

                                      //=================여기부터 공통 함수 구현=================//

Point* pushPoint(Point* head, int x, int y) { // (x, y)좌표를 저장하는 함수 (연결 리스트형 스택 자료구조 활용)

    Point* temp; // 임시 포인터 변수를 만듬
    temp = (Point*)malloc(sizeof(Point)); // 저장공간 할당

    temp->x = x; // x값 대입
    temp->y = y; // y값 대입
    temp->link = NULL; // 다음 연결 노드가 없다는 표시

    if (head == NULL) { // 공백리스트라면 진입
        head = temp;
    }
    else {
        temp->link = head;
        head = temp;
    }

    return head;

}

void peekPoint(Point* head, int* x, int*y) { // (x, y)좌표를 뱉는 함수 (연결 리스트형 스택 자료구조 활용)

                                             // 포인터 형으로 받은 x,y좌표에 값을 할당함
    *x = head->x;
    *y = head->y;

}

Stroke* makeStroke(Stroke* head, int r, Point* pt, CvScalar g) { // 논문상의 S들을 저장하는 함수

    Stroke *temp; // 임시 저장변수
    temp = (Stroke*)malloc(sizeof(Stroke));

    temp->r = r; // r값을 저장
    temp->pt = pt; // (x, y)좌표를 저장
    temp->g = g; // 색상 값을 저장
    temp->link = NULL; // 다음이 없다는 표시

    if (head == NULL) { // 공백 리스트라면
        head = temp;
    }
    else {
        temp->link = head;
        head = temp;
    }

    return head;

}

double difference(CvScalar a, CvScalar b) { // 색상의 차이값을 구하는 함수

    double distance; // 두 색의 차이를 3차원 공간에서의 거리와 같다고 생각하고 거리를 저장할 변수

                     // 두 색 사이의 거리 즉 차이를 구함 (논문에 기재되어있음, 수업시간에 교수님께서 설명해주심)
    distance = sqrt((a.val[0] - b.val[0])*(a.val[0] - b.val[0]) + (a.val[1] - b.val[1])*(a.val[1] - b.val[1]) + (a.val[2] - b.val[2])*(a.val[2] - b.val[2]));

    return fabs(distance); // 거리의 절대값을 반환 (사실 항상 양수일 수 밖에 없음)

}

Stroke* paintCanvas(Stroke* head, IplImage* canvas, int mode) { // 논문상의 canvas 이미지에 색칠하는 함수

    Stroke *temp = NULL, *move; // 임시 저장 변수

    move = head; // 연결 리스트의 맨 처음 변수를 저장

                 // 랜덤 선택 부분

    int num = rand() % 100; // 랜덤한 값을 가져옴


    for (int i = 0; i<num; i++) { // 랜덤한 값만큼 이동

        if (move->link == NULL) { // 가장 마지막 노드일 경우(1개의 노드밖에 없을 경우 진입함)
            temp = move;
            head = NULL;
            break;
        }
        else    if (move->link->link == NULL) // 다음 다음 다음 노드가 비어있다면 반복문 종료
            break;

        move = move->link; // 한칸 이동

    }

    if (temp == NULL) { // 위의 첫번째 반복문에서 첫번째 예외처리에 들어가지 않았을 경우가됨

                        // 랜덤한 수 만큼 이동해서 어떤 예외처리에도 걸리지 않고 반복문을 완료했음에도 불구하고
                        // 희박한 확률로 move가 가르키는 곳이 마지막 노드일경우
        if (move->link == NULL)
            return head;

        temp = move->link; // temp라는 사용할 노드가 move의 다음 노드를 가르키도록 함

        move->link = temp->link; // 데이터 유실을 방지하기 위해 move의 다음 노드가 다음 다음 노드가 되도록 지정
    }

    // 여기까지 랜덤 선택 부분


    // 2.1에서는 좌표가 하나이지만 2.2에서는 좌표가 여러개 이기 때문에 편의를 위해 하나의 변수로 설정해서 사용
    // 좌표들이 저장되어 있는 변수
    Point* pt = temp->pt;

    int preTempx, preTempy; // 2.2의 붓의 효과를 줄때 cvLine에서 필요한 이전 좌표값을 저장하기 위한 변수
    if (pt != NULL) // pt가 공백리스트일 경우를 방지
        peekPoint(pt, &preTempx, &preTempy); // 좌표값을 저장

                                             // 가지고 있는 모든 좌표가 없어질때까지
    while (pt != NULL) {

        int tempx, tempy; // 임시로 x,y좌표를 받을 변수
        peekPoint(pt, &tempx, &tempy); // pt로부터 x,y좌표를 빼냄

        if (mode == 0) // 2.1절 효과
            cvCircle(canvas, cvPoint(tempx, tempy), temp->r, temp->g, -1); // 기준좌표로부터 반지금 R만큼 원으로 채움
        else { // 2.2절 효과
            cvLine(canvas, cvPoint(preTempx, preTempy), cvPoint(tempx, tempy), temp->g, temp->r);

            // 다음 좌표에서는 현재좌표가 이전좌표가 되므로 이전좌표를 바꿔줌
            preTempx = tempx;
            preTempy = tempy;
        }


        pt = pt->link; // 다음 좌표로 이동(좌표가 여러개 있을 경우를 대비)
    }

    // 메모리에서 해제함
    free(temp);
    free(pt);

    //count--;

    return head;
}



void paintLayer(IplImage* canvas, IplImage* reference, int R, int mode, cv::VideoWriter &writer) { // 가장 기초가 되는 함수 (색칠할 x,y들을 구하는 등)

    CvSize size = cvGetSize(reference); // reference 즉 가우시안 필터가 적용된 원본 이미지의 사이즈 저장

    Stroke* S = NULL; // 각각의 브러쉬가 칠해질 좌표와 r값, 색상정보가 저장됨 (굉장히 많을 예정) (논문 상의 S)

    int fg = 1; // 논문상의 fg (여기서는 1로 설정)
    int grid = R*fg; // 하나의 범위가 되어줄 변수 (여기서는 1*R 값을 해줌)

                     // 2중 반복문으로써 하나의 grid^2 만큼의 영역을 반복하게 됨
    for (int y = size.height; y >= grid / 2; y -= grid) { // y값을 grid 값을 기준으로 반복함
        for (int x = grid / 2; x <= size.width; x += grid) { // x값을 grid 값을 기준으로 반복함

            int M_x1, M_x2, M_y1, M_y2; // 다른 하나의 구역인 M을 위한 변수 (논문 상의 M)
            M_x1 = x - grid / 2; M_x2 = x + grid / 2; // 현재 x 기반 +- grid/2, 즉 M구역 x축 시작과 끝 좌표를 저장
            M_y1 = y - grid / 2; M_y2 = y + grid / 2; // 위와 같음. M구역 y축 시작과 끝 좌표를 저장

                                                      // 구역 M이 이미지의 범위를 넘어갈 수 있기 때문에 예외처리
            if (M_x2 > size.width) M_x2 = size.width;
            if (M_y2 > size.height) M_y2 = size.height;

            double areaError = 0; // 색칠할 최고의 위치를 찾기 위한 변수 (논문 상의 areaError)

            int x1 = 0, y1 = 0; // 연결 리스트 S에 저장될 x,y 좌표를 저장할 변수 (논문 상의 x1, y1)
            double maxDistance = 0; // 최고 거리를 찾기 위한 변수 (x1, y1의 값을 찾기 위해)

                                    // 구역 M의 영역을 반복
            for (int j = M_y1; j<M_y2; j++) {

                for (int i = M_x1; i<M_x2; i++) {

                    CvScalar a, b; // 색칠할 canvas와 가우시안 필터가 적용된 reference 이미지의 거리를 구하기 위해 사용될 색 정보
                    a = cvGet2D(canvas, j, i); // 색칠할 canvas의 (i, j)의 좌표의 색정보를 가져옴
                    b = cvGet2D(reference, j, i); // 색칠할 reference의 (i, j)의 좌표의 색정보를 가져옴

                    double dif = difference(a, b); // a, b의 색의 거리를 구함 (논문 상의 D)
                    areaError = areaError + dif; // 구역 M의 색 거리를 모두 저장하기 위해

                                                 // 최고의 거리를 가지는 (x, y) 좌표를 찾기 위해
                    if (dif >= maxDistance) {
                        maxDistance = dif;
                        x1 = i;
                        y1 = j;
                    }


                }
            }

            areaError = areaError / (grid*grid);// 구역 M의 모든 색 거리 차이를 구한 값을 M구역 면적으로 나눔

                                                // 색을 칠하기 위해 기준이될 임이의 T값 (색의 차이가 T보다 크다면 칠하게 되므로 어느정도 색차이가 비슷해지면 해당 구역은 색칠하지 않게 됨)
            double T = 50;


            if (areaError > T) { // 색의 차이의 평균값을 T와 비교함

                Point * pt = NULL; // 좌표를 저장하게 될 변수(여러개일수도 있음)

                if (mode == 0) { // circle 모드일 경우
                    pt = pushPoint(pt, x1, y1); // 2.1은 좌표가 하나임으로 변도의 과정없이 좌표값을 대입함
                    S = makeStroke(S, R, pt, cvGet2D(reference, y1, x1)); // S의 연결 리스트에 stroke들 저장 (논문 상의 makeStroke와 add s to S)
                }
                else { // stroke 모드일 경우

                       // 주의 R이 아닌 R/2를 함
                    pt = makeSplineStroke(x1, y1, R, reference, canvas); // 구해진 x1, y1으로부터 곡률을 가지는 선을 만들게될 변수

                    if (pt != NULL)
                        S = makeStroke(S, R, pt, cvGet2D(reference, y1, x1)); // S의 연결 리스트에 stroke들 저장 (논문 상의 makeStroke와 add s to S)

                }

            }


        }
    }

    // 논문 상의 paint all strokes in S on the canvas
    int count = 0;
    int interval = 160 / R;
    while (S != NULL) {

        S = paintCanvas(S, canvas, mode); // canvas에 저장된 Stroke 들을 칠하는 함수
        ++count;
        if (count % interval == 0) {
            cv::Mat matimg = cv::cvarrToMat(canvas).clone();
            writer.write(matimg);
        }

    }

}

//=================================================================//
//============================여기부터 2.2절============================//


Point* makeSplineStroke(int x0, int y0, int R, IplImage* refImage, IplImage* canvas) { // 곡선모양의 선을 그리는 함수

    CvScalar strokeColor = cvGet2D(refImage, y0, x0); // refernceImage의 x,y에서의 색을 가져옴
    CvScalar strokeColor2 = cvGet2D(refImage, y0, x0); // gradientDirection에서 활용
    CvSize size = cvGetSize(refImage);

    Point* K = NULL; // 좌표들이 저장될 변수

    K = pushPoint(K, x0, y0); // 일단 삭제 (논문과 약간 다른 부분)
    int x = x0, y = y0; // 다음 좌표가 됨(이동한 좌표), 지금은 초기좌표를 설정
    int lastDx = 0, lastDy = 0; // 이전 좌표의 방향이 저장됨, 지금은 이전좌표가 없으니 0,0 설정

    int maxStrokeLength = 12, minStrokeLength = 3; // 선의 최소 길이와 최대길이

                                                   // 선의 최고길이 만큼 반복함
    for (int i = 0; i<maxStrokeLength; i++) {


        // i가 선의 최소길이 초과이고, (x, y)에서 refernceImage이미지와 canvas이미지의 색 차이가 refernceImage와 초기좌표의 refernceImage의 색차이보다 작다면
        if (i > minStrokeLength && fabs(difference(cvGet2D(refImage, y, x), cvGet2D(canvas, y, x))) < fabs(difference(cvGet2D(refImage, y, x), strokeColor))) {
            break;
        }

        // 여기 뭔가 추가해야함
        if (gradientMag(x, y, refImage) == 0) // 색의 변화량이 0인지 감지(사방이 같은색인지)
            break;


        double gx, gy; // 방향이 되어줄 좌표
        if (x - 1 >= 0 && x + 1<size.width && y - 1 >= 0 && y + 1<size.height) { // 범위 초과 감지

            gradientDirection(x, y, &gx, &gy, refImage); // G백터를 구하기 위해

        }
        else { // 범위를 초과하였다면 종료
            break;
        }
        double dx = -gy, dy = gx; // 논문상의 D1( L+PI/2를 의미함, G1으로부터 + 90도)

                                  // Di와 Di-1이 PI/2보다 작거나 같은지 확인하기 위해
                                  // 논문상의 D2가 됨(L-PI/2, G1으로부터 -90도)
        if (lastDx * dx + lastDy * dy < 0) {
            dx = -dx;
            dy = -dy;
        }


        double fc = 0.3; // 논문상의 fc (1보다 작은 임의의 상수)
        dx = fc * dx + (1.0 - fc) * lastDx;
        dy = fc * dy + (1.0 - fc) * lastDy;

        double d = sqrt(dx*dx + dy*dy);
        dx = dx / d;
        dy = dy / d;

        x = x + (double)R*dx;
        y = y + (double)R*dy;

        lastDx = dx;
        lastDy = dy;

        if (x<0 || x >= size.width || y<0 || y >= size.height) //이미지 범위 초과를 방지
            break;

        K = pushPoint(K, x, y); // K에 좌표 추가
        strokeColor2 = cvGet2D(refImage, y, x); // 다음 좌표의 색상값을 가져옴, gradientDirection에서 활용

    }

    return K;
}

double calculateColorAvg(CvScalar a) { // 입력된 색의 평균을 구함

    return (a.val[0] + a.val[1] + a.val[2]) / 3;

}

int gradientMag(int x, int y, IplImage* refImage) { // 기준좌표로부터 위,아래,양옆의 색변화가 없는지 확인을 위해

    CvSize size = cvGetSize(refImage); // 이미지 범위초과를 감지하기 위해

    if (x - 1 >= 0 && x + 1<size.width && y - 1 >= 0 && y + 1<size.height) { // 범위 초과 감지

        double right = calculateColorAvg(cvGet2D(refImage, y, x + 1));
        double left = calculateColorAvg(cvGet2D(refImage, y, x - 1));
        double down = calculateColorAvg(cvGet2D(refImage, y + 1, x));
        double up = calculateColorAvg(cvGet2D(refImage, y - 1, x));

        if (right == left && up == down) // 위, 아래, 양옆이 모두 기준 x,y좌표의 색과 같은색으로써 색의 변화량이 0이라면
            return 0;
        else
            return 1;


    }
    else { // 범위가 초과榮摸?
        return 0;
    }

}

void gradientDirection(int x, int y, double* gx, double* gy, IplImage* refImage) { // 논문상의 (gx, gy)를 구하기 위한 함수

    double right = calculateColorAvg(cvGet2D(refImage, y, x + 1));
    double left = calculateColorAvg(cvGet2D(refImage, y, x - 1));
    double down = calculateColorAvg(cvGet2D(refImage, y + 1, x));
    double up = calculateColorAvg(cvGet2D(refImage, y - 1, x));

    *gx = (right - left) / 2; // 기준 좌표에서 오른쪽의 색값과 왼쪽의 색값을 비교해 밝기 변화를 감지함
    *gy = (down - up) / 2; // 기준 좌표에서 아래쪽의 색값과 위쪽의 색값을 비교해 밝기 변화를 감지함

}

//=================================================================//

cv::Mat paint(IplImage* src, int * R, int R_length, int mode, IplImage *origin_canvas, cv::VideoWriter &writer) {
    CvSize size = cvGetSize(src); // 원본 이미지 사이즈

    IplImage* canvas = cvCreateImage(size, 8, 3); // 효과를 적용시킬 이미지 (논문상의 canvas(빈 이미지))
    // cvSet(canvas, cvScalar(255, 255, 255)); // 흰색 그림으로 설정 (흰 canvas)
    cvCopy(origin_canvas, canvas);
    IplImage* referenceImage = cvCreateImage(size, 8, 3); // 가우시안 필터를 적용시킬 이미지(논문상의 referenceImage)

    for (int i = 0; i<R_length; i++) { // 브러쉬의 개수 만큼 반복

        // 가우시안 필터를 적용함(설정 값은 브러쉬사이즈*3-1(홀수여야하기때문) (논문 상의 G(f Ri)))
        // 가우시안 필터값이 클수록 흐릿한 이미지를 가져올 수 있으므로
        // 브러쉬에 따라 가우시안 필터값이 작아질 수 있게끔 만들어줌(*5는 임의 값)
        // 2.1효과일때는 (*5)를 2.2효과일때는 (*3)을 값으로 대입함(더 좋은 결과값을 위해)
        if (mode == 0)
            cvSmooth(src, referenceImage, CV_GAUSSIAN, R[i] * 5 - 1);
        else if (mode == 1)
            cvSmooth(src, referenceImage, CV_GAUSSIAN, R[i] * 3 - 1);

        paintLayer(canvas, referenceImage, R[i], mode, writer); // dst라는 하나의 레이어 개념에 브러쉬 사이즈만큼 크기의 원을 채우기 위한 함수
    }
    
    cv::Mat matimg = cv::cvarrToMat(canvas).clone();
    
    cvReleaseImage(&canvas);
    cvReleaseImage(&referenceImage);
    
    return matimg;
}

cv::Mat oilpaint_transfer(const cv::Mat &image, std::string pre_video_name, std::string output_video_name) {
    IplImage* src = NULL;
    src = cvCreateImage(cvSize(image.cols,image.rows), 8, 3);
    IplImage ipltemp = image;
    cvCopy(&ipltemp, src);
    
    cv::VideoWriter writer(output_video_name.c_str(), CV_FOURCC('D', 'I', 'V', 'X'), 60, image.size(), 1);
    
    cv::VideoCapture reader(pre_video_name.c_str());
    cv::Mat frame;
    cv::Mat origin_canvas;
    while (reader.read(frame)) {
        cv::resize(frame, frame, image.size());
        writer.write(frame);
        origin_canvas = frame.clone();
    }
    reader.release();
    IplImage *canvas = NULL;
    canvas = cvCreateImage(cvSize(image.cols, image.rows), 8, 3);
    IplImage ipltemp2 = origin_canvas;
    cvCopy(&ipltemp2, canvas);
    
    int R1[] = { 8, 4, 4, 2, 2, 2, 2 };
    int R2[] = { 8, 16, 2, 2, 2, 2, 2 };
    int R3[] = { 8, 16, 4, 4, 2, 2, 2 };
    int R4[] = { 8, 16, 4, 2, 2, 2, 2 };
    
    int w = src->width;
    int h = src->height;
    int cmp = w * h;
    int R_length = 5;
    cv::Mat dst;
    
    switch (cmp / 80000) {
        case 0:
            R_length = sizeof(R1) / sizeof(int);
            dst = paint(src, R1, R_length, 1, canvas, writer);
            break;
        case 1:
            R_length = sizeof(R2) / sizeof(int);
            dst = paint(src, R2, R_length, 1, canvas, writer);
            break;
        case 2:
            R_length = sizeof(R3) / sizeof(int);
            dst = paint(src, R3, R_length, 1, canvas, writer);
            break;
        default:
            R_length = sizeof(R4) / sizeof(int);
            dst = paint(src, R4, R_length, 1, canvas, writer);
            break;
    }
    
    writer.release();
    
    cvReleaseImage(&src);
    return dst;
}

} // namespace oilpaint
} // namespace glamorous
