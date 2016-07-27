#pragma once

namespace pixelmap {

/* An Animation takes a visualization and maps it to LED segments */
class Animation {
  public:
    virtual void init()=0;
    virtual void update()=0;
    virtual void draw(float interpolation)=0;
};

} // end namespace pixelmap
