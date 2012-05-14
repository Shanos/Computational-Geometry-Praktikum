#ifndef CITY_H_
#define CITY_H_

#include <string>
#include "Point2d.h"

using namespace std;

class City {

private:

  string id;
  Point2d coord;

public:

  City( string id, float x, float y );
  virtual ~City();

  string getID() const {
    return id;
  }

  Point2d * getCoord() {
    return &coord;
  }

};

#endif /* CITY_H_ */
