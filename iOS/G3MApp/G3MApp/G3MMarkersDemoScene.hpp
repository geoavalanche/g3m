//
//  G3MMarkersDemoScene.hpp
//  G3MApp
//
//  Created by Diego Gomez Deck on 11/18/13.
//  Copyright (c) 2013 Igo Software SL. All rights reserved.
//

#ifndef __G3MApp__G3MMarkersDemoScene__
#define __G3MApp__G3MMarkersDemoScene__

#include "G3MDemoScene.hpp"

class Mark;

class G3MMarkersDemoScene : public G3MDemoScene {
private:
  long long _requestId;

protected:
  void rawActivate(const G3MContext* context);

  void rawSelectOption(const std::string& option,
                       int optionIndex);

public:

  G3MMarkersDemoScene(G3MDemoModel* model) :
  G3MDemoScene(model, "Markers", "", -1),
  _requestId(-1)
  {
  }

  void deactivate(const G3MContext* context);


  void addMark(Mark* mark);

};

#endif
