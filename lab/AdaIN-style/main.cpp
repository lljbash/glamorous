#include <iostream>
#include "style_trans.h"

using namespace glamorous;
using namespace cv;
using namespace std;

int main() {
    StyleTrans st("aaaaa");
    st.initialize("input/content/aaaaa.jpg", "input/style/shuimo3_g.jpg");
    st.apply();
    Mat res = st.get_dst();
    imshow("Res", res)
    waitKey(0);
    return 0;
}
