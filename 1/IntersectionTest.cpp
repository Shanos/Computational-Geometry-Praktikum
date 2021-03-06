#include "IntersectionTest.h"

#define DEBUG 0

IntersectionTest::IntersectionTest( string filename ) :
    filename( filename ), intersectionCount( 0 ) {
}

/**
 *  open file and kill me if i cant open it.
 */
int IntersectionTest::open() {

  file.open( filename.c_str(), ios::in );

  if( !file.is_open() ) {
    cerr << "File " << filename << " not found" << endl;
    exit( 1 );
  }
  return 0;
}

void IntersectionTest::calculateIntersections() {
  /* the file is now processed, the values are in the vector.
   * Now its time to calculate and start the clock
   * Note: the outer loop loops over all entries in the vector.
   * The inner loop loops over outerit+1.
   * So we do test two lines only one time with each other and not with itself.
   */
  int i = 0, j = 0;

  start = clock();
  /* intersection calculation with two iterator-loops */
  for( vector<Line *>::iterator outerit = lines.begin(); outerit != lines.end();
      outerit++ ) {
    j = i+1;
    for( vector<Line *>::iterator innerit = outerit + 1; innerit != lines.end();
        innerit++ ) {
      if( ( *outerit )->intersect( *innerit ) ) {
        intersectionCount++;
        cout << **outerit << "  " << **innerit << endl;
      }
      j++;
    }
    i++;
  }
  stop = clock();
}

void IntersectionTest::printResults() {
  cerr << "Number of Lines: " << lines.size() << endl << intersectionCount
      << " intersections" << endl << "calculated in " << getTime() << " seconds"
      << endl;

}
;

/* process the file line by line */
int IntersectionTest::parse() {
  float a[4]; /* a temporary array to store the values */
  string strline; /* one line of the file */
  open(); // first open the file, then read
  while( getline( file, strline ) ) {
    /* read line, parse float values and store in temporary array */
    sscanf( strline.c_str(), "%f %f %f %f", &a[0], &a[1], &a[2], &a[3] );
    /* create a new Line object and store it in the vector */
    Line* tmpin = new Line( a[0], a[1], a[2], a[3] );
    lines.push_back( tmpin );

#if DEBUG /* output */
    cout << "Input:  " << a[0] << " " << a[1] << " " << a[2] << " " << a[3]
    << endl;
    Line* tmpout = lines.back();
    cout << "Vector: " << tmpout->getA().getX() << " "
    << tmpout->getA().getY() << " " << tmpout->getB().getX() << " "
    << tmpout->getB().getY() << " " << endl;
#endif /* DEBUG */

  }
  return 0;
}

IntersectionTest::~IntersectionTest() {
  file.close();
  // TODO Auto-generated destructor stub
}

