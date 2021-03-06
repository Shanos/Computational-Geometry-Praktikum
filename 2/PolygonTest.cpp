#include "PolygonTest.h"

PolygonTest::PolygonTest( string filename ) :
    filename( filename ) {
}

PolygonTest::~PolygonTest() {
  file.close();
}

int PolygonTest::open() {

  file.open( filename.c_str(), ios::in );

  if( !file.is_open() ) {
    cerr << "File " << filename << " not found" << endl;
    exit( 1 );
  }
  return 0;
}

void PolygonTest::calculateAreas() const {
  for( vector<State *>::const_iterator stateit = states.begin();
      stateit != states.end(); stateit++ ) {
    (*stateit)->area();
  }
}

void PolygonTest::calculatePiP() const {

  for( vector<City *>::const_iterator cityit = cities.begin();
      cityit != cities.end(); cityit++ ) {

    for( vector<State *>::const_iterator stateit = states.begin();
        stateit != states.end(); stateit++ ) {

      if( ( *stateit )->cityInside( *cityit ) ) {
        cout << **cityit << " ist in " << **stateit << endl;
      }

    }
  }
}

void PolygonTest::printResults() const {
  /* parsed states */
  for( vector<State *>::const_iterator stateit = states.begin();
      stateit != states.end(); stateit++ ) {
    cout << "State " << **stateit << " contains "
        << ( *stateit )->getPolygons()->size() << " polygon(s) and ";

    for( vector<Polygon *>::iterator polyit =
        ( *stateit )->getPolygons()->begin();
        polyit != ( *stateit )->getPolygons()->end(); polyit++ ) {
      cout << ( *polyit )->getPoints()->size() << " ";
    }
    cout << " point(s)" << endl;
  }
  /* parsed cities */
  for( vector<City *>::const_iterator cityit = cities.begin();
      cityit != cities.end(); cityit++ ) {
    cout << "City " << **cityit << " at " << (*cityit)->getCoord() << endl;
  }
}

int PolygonTest::parse() {

  string strline; /* one line of the file */
  string id;
  float abs[2];
  float unk[2];
  open(); // first open the file, then read
  while( getline( file, strline ) ) {
    /* search for path */
    if( strline.find( "path" ) != string::npos ) {
      size_t pos1 = strline.find( "id=" );
      /*
       * The processing is split in two parts. The type of processing is determined with this if.
       * */
      if( pos1 != string::npos ) {
        /*
         * we found a id= in the same line. process the state.
         */

        /*
         * Get the id of the state and create a new object.
         */
        size_t pos2 = strline.find( " ", pos1 + 1 );
        id = strline.substr( pos1 + 4, pos2 - ( pos1 + 4 ) - 1 );
        State * tmpstate = new State( id );

        /*
         * Now start with the polygons.
         */
        int i = 0;
        Polygon * tmppoly = new Polygon();
        Polygon * bigpoly;
        while( getline( file, strline ) ) {
          if( strline.find( "/>" ) != string::npos ) {
            states.push_back( tmpstate );
            break;
          }
          else {
            string ctype = strline.substr( 0, 1 );
            const char * type = ctype.c_str();
            string cval = strline.substr( 1 );
            const char * val = cval.c_str();
            sscanf( val, "%f,%f", &unk[0], &unk[1] );
            switch( *type ) {
            /* z/Z is the delimiter for a new polygon */
            case ( 'Z' ):
            case ( 'z' ):
              tmpstate->addPolygon( tmppoly );
              if(!i) { // if i is null store the first (biggest) polygon to bigpoly pointer.
                bigpoly = tmppoly;
              }
              tmppoly = new Polygon();
              i = 1;
              break;
              /* M/L are absolute coordinates */
            case ( 'M' ):
            case ( 'L' ):
              abs[0] = unk[0];
              abs[1] = unk[1];
              if(i){
                Point2d * h = new Point2d(abs[0], abs[1]);
                if(bigpoly->pip(*h)){
                  tmppoly->setInner();
                  //cout << id << " inner! Punkt: " << *h << endl;
                  i = 0;
                }
              }
              tmppoly->addPoint( abs[0], abs[1] );
              break;
              /* m/l are relative coordinates */
            case ( 'm' ):
            case ( 'l' ): /* abs contains the last coordinates, unk contains relative coordinates */
              abs[0] += unk[0];
              abs[1] += unk[1];
              tmppoly->addPoint( abs[0], abs[1] );
              break;
            default:
              cerr << "Parser Error: not implemented: " << *type << endl;
              break;
            }
          }
        }
      }
      else { /*----------------------------------------------------------------------*/
        /*
         * process a city.
         */
        while( getline( file, strline ) ) {
          size_t pos1 = strline.find( "id=" );
          if( pos1 != string::npos ) {
            size_t pos2 = strline.find( "\"", pos1 + 4 );
            id = strline.substr( pos1 + 4, pos2 - ( pos1 + 4 ) );
            getline( file, strline );
            pos1 = strline.find( "\"" );
            pos2 = strline.find( "\"", pos1 + 1 );
            string cval = strline.substr( pos1 + 1, pos2 - 1 );
            const char * val = cval.c_str();
            sscanf( val, "%f", &abs[0] );
            getline( file, strline );
            pos1 = strline.find( "\"" );
            pos2 = strline.find( "\"", pos1 + 1 );
            cval = strline.substr( pos1 + 1, pos2 - 1 );
            val = cval.c_str();
            sscanf( val, "%f", &abs[1] );
            City * tmpcity = new City( id, abs[0], abs[1] );
            cities.push_back( tmpcity );
            break; //FIXME: what to break here?
          }
        }
      }
    }
  }
  return 0;
}

