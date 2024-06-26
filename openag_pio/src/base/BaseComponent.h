#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H


class BaseComponent
{
  public:
    /**
     * Public properties
    */
  virtual void Setup()=0;
  virtual void Loop(const float dt)=0;

};

#endif
