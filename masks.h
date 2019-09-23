//
// Created by Henri on 23/09/2019.
//

#ifndef ATELIER_INDEXATION_MASKS_H
#define ATELIER_INDEXATION_MASKS_H

const int impulse_response[3][3] = {
        {1,1,1},
        {1,1,1},
        {1,1,1}
};

const int horizontal_gradient[3][3] = {
        {-1,0,1},
        {-2,0,2},
        {-1,0,1}
};

const int vertical_gradient[3][3] = {
        {-1,-2,-1},
        {0,0,0},
        {1,2,1}
};

const int averaging_filter[3][3] = {
        {1/9,1/9,1/9},
        {1/9,1/9,1/9},
        {1/9,1/9,1/9}
};
#endif //ATELIER_INDEXATION_MASKS_H
