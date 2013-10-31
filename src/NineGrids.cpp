#include <cmath>
#include <cstdio>
#include <cfloat>
#include <algorithm>

using namespace std;

#include <NineGrids.h>
#include <RecogUtils.h>

/**
 * Nine-grid scanning algorithm for digital recognizing.
 * TODO Test this function.
 * @author EUYUIL
 * @date 2012-06-21
 */

bool NineGridScan(const char *m, unsigned int w, unsigned int h, NG_RESULT &rs)
{
    if (w < 1 || h < 1)
        return false;

    unsigned int w1, w2, h1, h2, dw, dh;
    GetMatrixBound(m, w, h, w1, w2, h1, h2);
    dw = w2 - w1; dh = h2 - h1;

    const unsigned int wb[9] = {w1, w1 + dw / 9, w1 + 2 * dw / 9,w1 + 3 * dw / 9, w1 + 4 * dw / 9,w1 + 5 * dw / 9, w1 + 6 * dw / 9,w1 + 7 * dw / 9,w1 + 8 * dw / 9};
    const unsigned int we[9] = {w1 + dw / 9, w1 + 2 * dw / 9,w1 + 3 * dw / 9, w1 + 4 * dw / 9,w1 + 5 * dw / 9, w1 + 6 * dw / 9,w1 + 7 * dw / 9,w1 + 8 * dw / 9, w1 + dw};
    const unsigned int hb[9] = {h1, h1 + dh / 9, h1 + 2 * dh / 9,h1 + 3 * dh / 9, h1 + 4 * dh / 9,h1 + 5 * dh / 9, h1 + 6 * dh / 9,h1 + 7 * dh / 9,h1 + 8 * dh / 9};
    const unsigned int he[9] = {h1 + dh / 9, h1 + 2 * dh / 9,h1 + 3 * dh / 9, h1 + 4 * dh / 9,h1 + 5 * dh / 9, h1 + 6 * dw / 9,h1 + 7 * dh / 9,h1 + 8 * dh / 9, h1 + dh};

    fprintf(stderr, "NineGrids:");

    for (unsigned int i = 0; i < 9; ++i)
    {
        for (unsigned int j = 0; j < 9; ++j)
        {
            unsigned int cnt = 0;

            for (unsigned int r = hb[i]; r < he[i]; ++r)
            {
                for (unsigned int c = wb[j]; c < we[j]; ++c)
                {
                    const char *p = m + c + r * w;

                    if (*p != ' ')
                        ++cnt;
                }
            }

            unsigned int area = (he[i] - hb[i]) * (we[j] - wb[j]);
            rs[i][j] = double(cnt) / double(area);

            fprintf(stderr, " %lf", rs[i][j]);
        }
    }

    fprintf(stderr, "\n");

    return true;
}

//NG_COMP_RESULT NineGridCompare(const NG_RESULT &ia, const NG_RESULT &ib)
//{
//    const double *a = (const double *)ia, *b = (const double *)ib;
//    double meana = 0.0, meanb = 0.0, meana2 = 0.0, meanb2 = 0.0;
//
//    for (unsigned int i = 0; i < 9; ++i)
//        meana += a[i], meana2 += a[i] * a[i],
//        meanb += b[i], meanb2 += b[i] * b[i];
//
//    meana /= 9.0; meanb /= 9.0; meana2 /= 9.0; meanb2 /= 9.0;
//
//    double sigmaa = sqrt(meana2 - meana * meana);
//    double sigmab = sqrt(meanb2 - meanb * meanb);
//
//    if (sigmaa < DBL_EPSILON || sigmab < DBL_EPSILON)
//        return 1.0;
//
//    double result = 0.0;
//
//    for (unsigned int i = 0; i < 9; ++i)
//        result += (a[i] - meana) * (b[i] - meanb);
//
//    result /= sigmaa; result /= sigmab; result /= 9.0;
//
//    return (result + 1.0) / 2.0;
//}
