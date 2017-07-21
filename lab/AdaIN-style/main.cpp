#include <iostream>
#include "style_trans.h"

using namespace glamorous;
using namespace cv;
using namespace std;

int main() {
    StyleTrans st("aaaaa");
    st.initialize("/home/lljbash/b.jpg", "/home/lljbash/data/shuimo/caimo/11-14060Q0223EX.jpg");
    st.apply();
    Mat res = st.get_dst();
    imshow("Res", res);
    waitKey(0);
    return 0;
}
