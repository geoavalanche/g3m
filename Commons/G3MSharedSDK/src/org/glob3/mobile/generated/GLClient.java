

package org.glob3.mobile.generated;

//
//  GLClient.hpp
//  G3MiOSSDK
//
//  Created by Jose Miguel SN on 10/05/13.
//
//


public class GLClient {
   protected GLClient _parent;


   public GLClient() {
      _parent = null;
   }


   public void dispose() {
   }


   //  //Idle if this is not a drawable client
   //  virtual void getGLGlobalStateAndGPUProgramState(GLGlobalState** GLGlobalState, GPUProgramState** progState){
   //    (*GLGlobalState) = NULL;
   //    (*progState) = NULL;
   //  }

   public GLGlobalState getGLGlobalState() {
      return null;
   }


   public GPUProgramState getGPUProgramState() {
      return null;
   }


   public void notifyGLClientChildrenParentHasChanged() {
      //TIPICALLY: _mesh->actualizeGLGlobalState(this);
   } //Idle if this is a drawable client


   //Implemented if the node modifies the GLGlobalState
   public void modifyGLGlobalState(final GLGlobalState GLGlobalState) {
   }


   //Implemented if the node modifies the GPUProgramState
   public void modifyGPUProgramState(final GPUProgramState progState) {
   }


   /**
    * Invoked by parent to change my GLGlobalState and GPUProgramState
    */
   public final void actualizeGLGlobalState(final GLClient parent) {
      _parent = parent;
      notifyGLClientChildrenParentHasChanged();

      final GLGlobalState GLGlobalState = getGLGlobalState();
      final GPUProgramState programState = getGPUProgramState();
      //    getGLGlobalStateAndGPUProgramState(&GLGlobalState, &programState);
      if ((GLGlobalState != null) && (programState != null)) {
         //We are a drawable client
         modifyGLGlobalStatesHierarchy(GLGlobalState, programState);
      }

   }


   /**
    * Modifying current children's GLGlobalState and GPUProgramState
    */
   public final void modifyGLGlobalStatesHierarchy(final GLGlobalState GLGlobalState,
                                                   final GPUProgramState programState) {
      if (_parent != null) {
         _parent.modifyGLGlobalStatesHierarchy(GLGlobalState, programState);
      }
      modifyGLGlobalState(GLGlobalState);
      modifyGPUProgramState(programState);
   }
}
