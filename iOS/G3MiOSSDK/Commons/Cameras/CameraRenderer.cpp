//
//  CameraRenderer.cpp
//  G3MiOSSDK
//
//  Created by Agustín Trujillo Pino on 30/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "CameraRenderer.hpp"
#include "Camera.hpp"
#include "CameraEventHandler.hpp"
#include "TouchEvent.hpp"

CameraRenderer::~CameraRenderer() {
  delete _cameraContext;
  const int handlersSize = _handlers.size();
  for (int i = 0; i < handlersSize; i++) {
    CameraEventHandler* handler = _handlers[i];
    delete handler;
  }
}

void CameraRenderer::initialize(const G3MContext* context) {
  //_logger = ic->getLogger();
  //cameraContext = new CameraContext(
}

void CameraRenderer::onResizeViewportEvent(const G3MEventContext* ec,
                                           int width, int height) {
  //  moved to G3MWidget::onResizeViewportEvent
  //  if (_cameraContext != NULL) {
  //    _cameraContext->getNextCamera()->resizeViewport(width, height);
  //  }
}


void CameraRenderer::render(const G3MRenderContext* rc,
                            const GLState& parentState) {
  // create the CameraContext
  if (_cameraContext == NULL) {
    _cameraContext = new CameraContext(None, rc->getNextCamera());
  }

  // render camera object
  rc->getCurrentCamera()->render(rc, parentState);

  const int handlersSize = _handlers.size();
  for (unsigned int i = 0; i < handlersSize; i++) {
    _handlers[i]->render(rc, _cameraContext);
  }
}

bool CameraRenderer::onTouchEvent(const G3MEventContext* ec,
                                  const TouchEvent* touchEvent) {
  if (_processTouchEvents) {
    // abort all the camera effect currently running
    if (touchEvent->getType() == Down){
      EffectTarget* target = _cameraContext->getNextCamera()->getEffectTarget();
      ec->getEffectsScheduler()->cancelAllEffectsFor(target);
    }

    // pass the event to all the handlers
    const int handlersSize = _handlers.size();
    for (unsigned int i = 0; i < handlersSize; i++) {
      if (_handlers[i]->onTouchEvent(ec, touchEvent, _cameraContext)) {
        return true;
      }
    }
  }

  // if no handler processed the event, return not-handled
  return false;
}
