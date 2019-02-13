#ifndef CUBECOORDINATE_HH
#define CUBECOORDINATE_HH

/**
 * @file
 * @brief Point in cube coordinate system.
 */

namespace Common {

/**
 * @brief Used to describe a location in cube coordinate system.
 *
 */
struct CubeCoordinate {
    int x;
    int y;
    int z;

    /**
     * @brief Constructor.
     * @param ux X-coordinate.
     * @param uy Y-coordinate.
     * @param uz Z-coordinate.
     */
    CubeCoordinate(int newX,int newY,int newZ) {

        x = newX;
        y = newY;
        z = newZ;

    }

    /**
     * @brief CubeCoordinate deafult constructor
     */
    CubeCoordinate(){}

    /**
     * @brief Overloaded operation to compare the point to another one.
     * @param str Point, which this point is compared to.
     * @return True, if this point is "smaller" than the compared point. Otherwise false.
     */
    bool operator < (const CubeCoordinate& str) const {

        if (this->x != str.x) {
            return (this->x < str.x);
        } else if (this->y != str.y) {
            return (this->y < str.y);
        } else {
            return (this->z < str.z);
        }

    }

    /**
     * @brief Overloaded operation to compare the point to another one.
     * @param str Point, which this point is compared to.
     * @return True, if both the coordinates of the points are the same. Otherwise false.
     */
    bool operator == (const CubeCoordinate& str) const {

        return (this->x == str.x and this->y == str.y and this->z == str.z);

    }

};

}

#endif
