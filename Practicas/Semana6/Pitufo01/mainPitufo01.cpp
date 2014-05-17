#include "Ogre\Ogre.h"
#include "OIS\OIS.h"


class AppFrameListener : public Ogre::FrameListener
{

private:

OIS::InputManager* _InputManager;
OIS::Keyboard* _Keyboard;
OIS::Mouse* _Mouse;
Ogre::Camera* _Cam;
float _movementspeed;


public:

AppFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam){

OIS::ParamList parameters;
unsigned int windowHandle = 0;
std::ostringstream windowHandleString;
win->getCustomAttribute("WINDOW", &windowHandle);
windowHandleString << windowHandle;
parameters.insert(std::make_pair("WINDOW", windowHandleString.str()));
_InputManager = OIS::InputManager::createInputSystem(parameters);
_Keyboard = static_cast<OIS::Keyboard*>(_InputManager->createInputObject( OIS::OISKeyboard, false ));
_Mouse = static_cast<OIS::Mouse*>(_InputManager->createInputObject( OIS::OISMouse, false ));

_Cam = cam;
_movementspeed = 1200.0f;
}


~AppFrameListener(){
_InputManager->destroyInputObject(_Keyboard);
_InputManager->destroyInputObject(_Mouse);
OIS::InputManager::destroyInputSystem(_InputManager);
}


bool frameStarted(const Ogre::FrameEvent& evt){

Ogre::Vector3 translate(0,0,0);

_Keyboard->capture();

if(_Keyboard->isKeyDown(OIS::KC_ESCAPE)){
return false;
}


if(_Keyboard->isKeyDown(OIS::KC_W)){
translate += Ogre::Vector3(0,0,-1);
}

if(_Keyboard->isKeyDown(OIS::KC_S)){
translate += Ogre::Vector3(0,0,1);
}

if(_Keyboard->isKeyDown(OIS::KC_A)){
translate += Ogre::Vector3(-1,0,0);
}

if(_Keyboard->isKeyDown(OIS::KC_D)){
translate += Ogre::Vector3(1,0,0);
}

_Cam->moveRelative(translate*evt.timeSinceLastFrame * _movementspeed);


_Mouse->capture();
float rotX = _Mouse->getMouseState().X.rel * evt.timeSinceLastFrame* -1;
float rotY = _Mouse->getMouseState().Y.rel * evt.timeSinceLastFrame * -1;
_Cam->yaw(Ogre::Radian(rotX));
_Cam->pitch(Ogre::Radian(rotY));

return true;
}


bool frameEnded(const Ogre::FrameEvent& evt){
//std::cout << "Frame ended" << std::endl;
return true;
}

bool frameRenderingQueued(const Ogre::FrameEvent& evt){
//std::cout << "Frame queued" << std::endl;
return true;
}


};



class cAplicacion {

private:
Ogre::SceneManager* _sceneManager;
Ogre::Root* _root;
AppFrameListener* _listener;
Ogre::Camera* camera;
bool _keepRunning;


public:

// Constructor de la clase
cAplicacion(){
_sceneManager = NULL;
_root = NULL;
_listener = NULL;
}

~cAplicacion(){
delete _root;
delete _listener;
}

// Funcion que permite cargar los recursos de OGRE esto simplifica tener que cargarlos uno por uno
void loadResources(){
Ogre::ConfigFile cf;
cf.load("resources_d.cfg");

Ogre::ConfigFile::SectionIterator sectionIter = cf.getSectionIterator();
Ogre::String sectionName, typeName, dataname;

while (sectionIter.hasMoreElements()){
sectionName = sectionIter.peekNextKey();
Ogre::ConfigFile::SettingsMultiMap *settings = sectionIter.getNext();
Ogre::ConfigFile::SettingsMultiMap::iterator i;

for (i = settings->begin(); i != settings->end(); ++i){
typeName = i->first;
dataname = i->second;
Ogre::ResourceGroupManager::getSingleton().addResourceLocation(dataname, typeName, sectionName);
}

}

Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}


// funcion que configura la camara
void createCamera(Ogre::RenderWindow* window){
camera = _sceneManager->createCamera("Camera");
camera->setPosition(Ogre::Vector3(800,800,800));
camera->lookAt(Ogre::Vector3(0,0,0));
camera->setNearClipDistance(5);

Ogre::Viewport* viewport = window->addViewport(camera);
viewport->setBackgroundColour(Ogre::ColourValue(0.0,0.0,0.0));
camera->setAspectRatio(Ogre::Real(viewport->getActualWidth())/Ogre::Real(viewport->getActualHeight()));

}

// funcion donde se coloca lo que se desea desplegar.
void createScene(){

Ogre::Entity* entEscena01 = _sceneManager->createEntity("casaPitufo01.mesh");
_sceneManager->getRootSceneNode()->attachObject(entEscena01);

Ogre::Entity* entEscena02 = _sceneManager->createEntity("casaPitufo02.mesh");
_sceneManager->getRootSceneNode()->attachObject(entEscena02);

Ogre::Entity* entEscena03 = _sceneManager->createEntity("casaPitufo03.mesh");
_sceneManager->getRootSceneNode()->attachObject(entEscena03);

Ogre::Entity* entEscena04 = _sceneManager->createEntity("terrenoPitufo01.mesh");
_sceneManager->getRootSceneNode()->attachObject(entEscena04);

Ogre::Entity* entEscena05 = _sceneManager->createEntity("rocasPitufo01.mesh");
_sceneManager->getRootSceneNode()->attachObject(entEscena05);


}


void renderOneFrame(){
Ogre::WindowEventUtilities::messagePump();
_keepRunning = _root->renderOneFrame();
}

bool keepRunning(){
return _keepRunning;
}

//Funcion que inicializa la aplicacion
int startup(){
_root = new Ogre::Root("plugins_d.cfg");

//Muestra la ventana inicial...

//if(!_root->showConfigDialog()){
//	return -1;
//}


// Evitan cargar la ventana inicial...
Ogre::RenderSystem* _rs = _root->getRenderSystemByName("Direct3D9 Rendering Subsystem");
// or use "OpenGL Rendering Subsystem"
_root->setRenderSystem(_rs);
_rs->setConfigOption("Full Screen", "No");
_rs->setConfigOption("Video Mode", "800 x 600 @ 32-bit colour");
_rs->setConfigOption("FSAA", "0");
_rs->setConfigOption("Floating-point mode", "Fastest");
_rs->setConfigOption("Use Multihead", "Auto");
_rs->setConfigOption("VSync", "No");
_rs->setConfigOption("VSync Interval", "1");



Ogre::RenderWindow* window = _root->initialise(true,"Ventana de Ogre3D");
_sceneManager = _root->createSceneManager(Ogre::ST_GENERIC);


loadResources();
createScene();
createCamera(window);

_listener = new AppFrameListener(window,camera);
_root->addFrameListener(_listener);


//_root->startRendering();
return 0;


}


};




int main (void)
{
cAplicacion app;
app.startup();

while(app.keepRunning()){
app.renderOneFrame();
}

return 0;

}