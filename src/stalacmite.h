//
//  stalacmite.h
//  Granular_Testing
//
//  Created by Nicholas Gang on 11/8/15.
//
//

#ifndef stalacmite_h
#define stalacmite_h


#endif /* stalacmite_h */

//Attempt at a stalactite class
class stalacmite {
    
public:
    //Methods
    void update();
    void draw();
    ofVec3f getPosition();
    void perturb();
    
    //Physical properties
    float x;
    float y;
    float rad;
    
    //Constructor
    stalacmite();
    
    //Destructor
    ~stalacmite();
    
private:
    
};