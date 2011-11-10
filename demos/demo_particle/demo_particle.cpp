#include <math.h>

#include <april/main.h>
#include <april/RenderSystem.h>
#include <april/Timer.h>
#include <april/Window.h>
#include <aprilparticle/aprilparticle.h>
#include <aprilparticle/Affectors.h>
#include <aprilparticle/Emitter.h>
#include <aprilparticle/System.h>
#include <gtypes/Matrix3.h>
#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#define AFFECTOR_FORCE_1 "force1"
#define AFFECTOR_FORCE_2 "force2"
#define AFFECTOR_FORCE_3 "force3"
#define AFFECTOR_FORCE_4 "force4"

gvec2 screen(1024.0f, 768.0f);

april::Texture* redParticle;
april::Texture* greenParticle;

aprilparticle::System* flame = NULL;
aprilparticle::System* bubbles = NULL;
aprilparticle::System* vortex = NULL;
aprilparticle::System* rain = NULL;
aprilparticle::System* quazar = NULL;
aprilparticle::System* twirl = NULL;

april::ColoredVertex grid[44];

void setupGrid(float spacing)
{
	int i = 0;
	for (float s = -5 * spacing; s <= 5 * spacing; i++, s += spacing)
	{
		grid[i * 4 + 0].set(-5 * spacing, 0.0f, s);
		grid[i * 4 + 0].color = 0x777777FF;
		grid[i * 4 + 1].set(5 * spacing, 0.0f, s);
		grid[i * 4 + 1].color = 0x777777FF;
		grid[i * 4 + 2].set(s, 0.0f, -5 * spacing);
		grid[i * 4 + 2].color = 0x777777FF;
		grid[i * 4 + 3].set(s, 0.0f, 5 * spacing);
		grid[i * 4 + 3].color = 0x777777FF;
	}
}

void drawGrid()
{
	april::rendersys->render(april::LineList, grid, 44);
}

bool render(float k)
{
	april::rendersys->clear(true, false);
	static float angle = 0.0f;
	angle += k * 90.0f;
    
	april::rendersys->setPerspective(60.0f, screen.x / screen.y, 0.1f, 100.0f);
	
	gvec3 pos(0.0f, 18.0f, 25.0f);
	gmat3 rot;
	rot.setRotation3D(0.0f, 1.0f, 0.0f, angle * 0.2f);
	pos = rot * pos;
	april::rendersys->lookAt(pos, gvec3(0.0f, 0.0f, 0.0f), gvec3(0.0f, 1.0f, 0.0f));
	drawGrid();
	
	twirl->getAffector<aprilparticle::Affectors::DirectionalForceField>(AFFECTOR_FORCE_1)->setDirection(gvec3(sin(angle * 0.06666667f), 0.0f, cos(angle * 0.03333333f)));
	twirl->getAffector<aprilparticle::Affectors::DirectionalForceField>(AFFECTOR_FORCE_2)->setDirection(gvec3(sin(angle * 0.06666667f), sin(angle * 0.03333333f), 0.0f));
	twirl->getAffector<aprilparticle::Affectors::DirectionalForceField>(AFFECTOR_FORCE_3)->setDirection(gvec3(cos(angle * 0.03333333f), 0.0f, sin(angle * 0.06666667f)));
	twirl->getAffector<aprilparticle::Affectors::DirectionalForceField>(AFFECTOR_FORCE_4)->setDirection(gvec3(sin(angle * 0.03333333f), sin(angle * 0.06666667f), 0.0f));
	
	flame->update(k);
	bubbles->update(k);
	vortex->update(k);
	rain->update(k);
	quazar->update(k);
	twirl->update(k);
	
	flame->draw(pos);
	bubbles->draw(pos);
	vortex->draw(pos);
	rain->draw(pos);
	quazar->draw(pos);
	twirl->draw(pos);
	
	return true;
}

void setupFlame()
{
	flame = new aprilparticle::System();
	// emitters
	aprilparticle::Emitter* emitter = new aprilparticle::Emitter();
	flame->registerEmitter(emitter);
	// textures
	april::Texture* fire = april::rendersys->loadTexture("../media/fire_particle.png");
	flame->registerTexture(fire);
	emitter->setTexture(fire);
	// affectors
	aprilparticle::Affectors::MultiColorChanger* colorChanger = new aprilparticle::Affectors::MultiColorChanger();
	flame->registerAffector(colorChanger);
	emitter->addAffector(colorChanger);
	aprilparticle::Affectors::LinearDirectionalForce* linearforce = new aprilparticle::Affectors::LinearDirectionalForce(gvec3(0.0f, 1.0f, 0.0f), 0.018f);
	flame->registerAffector(linearforce);
	emitter->addAffector(linearforce);
	aprilparticle::Affectors::Rotator* rotator = new aprilparticle::Affectors::Rotator(50.0f);
	flame->registerAffector(rotator);
	emitter->addAffector(rotator);

	flame->setPosition(-10.0f, 0.0f, 0.0f);

	emitter->setType(aprilparticle::Emitter::HollowSphere);
	emitter->setDimensions(2.0f, 4.0f, 2.0f);
	emitter->setBlendMode(april::ADD);
	emitter->setEmissionRate(32.0f);
	emitter->setLimit(128);
	emitter->setLife(4.0f);
	emitter->setScaleRange(2.4f, 3.4f);
	emitter->setAngleRange(0.0f, 360.0f);

	hmap<float, april::Color> colors;
	colors[0.0f] = april::Color(0xFF000000);
	colors[0.1f] = april::Color(0xFF6432F3);
	colors[0.4f] = april::Color(0xFF7F3FAF);
	colors[0.75f] = april::Color(0x7F7F7FAF);
	colors[1.0f] = april::Color(0x7F7F7F00);
	colorChanger->setColorTimings(colors);
}

void setupBubbles()
{
	bubbles = new aprilparticle::System();
	// emitters
	aprilparticle::Emitter* emitter = new aprilparticle::Emitter();
	bubbles->registerEmitter(emitter);
	// textures
	april::Texture* bubble = april::rendersys->loadTexture("../media/bubble.png");
	bubbles->registerTexture(bubble);
	emitter->setTexture(bubble);
	// affectors
	aprilparticle::Affectors::MultiColorChanger* colorChanger = new aprilparticle::Affectors::MultiColorChanger();
	bubbles->registerAffector(colorChanger);
	emitter->addAffector(colorChanger);
	aprilparticle::Affectors::DirectionalForceField* directionalForce = new aprilparticle::Affectors::DirectionalForceField(gvec3(0.0f, 1.0f, 0.0f), gvec3(0.0f, 1.0f, 0.0f), 4.0f);
	bubbles->registerAffector(directionalForce);
	emitter->addAffector(directionalForce);

	emitter->setType(aprilparticle::Emitter::Cylinder);
	emitter->setDimensions(5.0f, 4.0f, 5.0f);
	emitter->setBlendMode(april::DEFAULT);
	emitter->setEmissionRate(50.0f);
	emitter->setLimit(300);
	emitter->setLife(6.0f);
	emitter->setScaleRange(0.4f, 0.8f);

	hmap<float, april::Color> colors;
	colors[0.0f] = april::Color(0xFFFFFF00);
	colors[0.04f] = april::Color(0xFFFFFFFF);
	colors[0.995f] = april::Color(0xFFFFFFFF);
	colors[1.0f] = april::Color(0xFFFFFF00);
	colorChanger->setColorTimings(colors);
}

void setupVortex()
{
	vortex = new aprilparticle::System();
	// emitters
	aprilparticle::Emitter* emitter = new aprilparticle::Emitter();
	vortex->registerEmitter(emitter);
	// textures
	emitter->setTexture(redParticle); // texture is shared among several particle systems
	// affectors
	aprilparticle::Affectors::Swirl* swirl = new aprilparticle::Affectors::Swirl(gvec3(10.0f, 0.0f, 0.0f), gvec3(0.0f, 1.0f, 0.0f), 0.2f, 8.0f, true);
	vortex->registerAffector(swirl);
	emitter->addAffector(swirl);
	aprilparticle::Affectors::Attractor* attractor = new aprilparticle::Affectors::Attractor(gvec3(10.0f, 0.0f, 0.0f), 2.0f);
	vortex->registerAffector(attractor);
	emitter->addAffector(attractor);

	vortex->setPosition(10.0f, 0.0f, 0.0f);

	emitter->setType(aprilparticle::Emitter::Sphere);
	emitter->setDimensions(8.0f, 0.0f, 8.0f);
	emitter->setBlendMode(april::ADD);
	emitter->setEmissionRate(200.0f);
	emitter->setLimit(800);
	emitter->setLife(2.0f);
	emitter->setScale(0.4f);
}

void setupRain()
{
	rain = new aprilparticle::System();
	// emitters
	aprilparticle::Emitter* emitter = new aprilparticle::Emitter();
	rain->registerEmitter(emitter);
	// textures
	april::Texture* rainDrop = april::rendersys->loadTexture("../media/rain_drop.png");
	bubbles->registerTexture(rainDrop);
	emitter->setTexture(rainDrop);
	// making the emitter responsible for these affectors
	aprilparticle::Affectors::LinearDirectionalForce* gravity = new aprilparticle::Affectors::LinearDirectionalForce(gvec3(0.0f, 1.0f, 0.0f), -0.012f);
	rain->registerAffector(gravity);
	emitter->addAffector(gravity);

	rain->setPosition(-10.0f, -10.0f, 6.0f);

	emitter->setType(aprilparticle::Emitter::Box);
	emitter->setDimensions(6.0f, 0.0f, 6.0f);
	emitter->setBlendMode(april::ADD);
	emitter->setEmissionRate(60.0f);
	emitter->setLimit(240);
	emitter->setLife(4.0f);
	emitter->setScaleRange(0.1f, 0.4f);
}

void setupQuazar()
{
	quazar = new aprilparticle::System();
	// emitters
	aprilparticle::Emitter* discEmitter = new aprilparticle::Emitter();
	quazar->registerEmitter(discEmitter);
	aprilparticle::Emitter* jetTopEmitter = new aprilparticle::Emitter();
	quazar->registerEmitter(jetTopEmitter);
	aprilparticle::Emitter* jetBottomEmitter = new aprilparticle::Emitter();
	quazar->registerEmitter(jetBottomEmitter);
	// textures
	april::Texture* blueParticle = april::rendersys->loadTexture("../media/blue_particle.png");
	quazar->registerTexture(blueParticle);
	jetTopEmitter->setTexture(blueParticle);
	jetBottomEmitter->setTexture(blueParticle);
	discEmitter->setTexture(greenParticle); // texture is shared among several particle systems
	// affectors
	aprilparticle::Affectors::Attractor* gravity = new aprilparticle::Affectors::Attractor(gvec3(0.0f, 0.0f, 10.0f), 2.0f);
	quazar->registerAffector(gravity);
	discEmitter->addAffector(gravity);
	aprilparticle::Affectors::Swirl* spin = new aprilparticle::Affectors::Swirl(gvec3(0.0f, 0.0f, 10.0f), gvec3(1.0f, 0.0f, 0.0f), 0.1f, 8.0f, true);
	quazar->registerAffector(spin);
	discEmitter->addAffector(spin);
	aprilparticle::Affectors::LinearDirectionalForce* top = new aprilparticle::Affectors::LinearDirectionalForce(gvec3(1.0f, 0.0f, 0.0f), 0.024f);
	quazar->registerAffector(top);
	jetTopEmitter->addAffector(top);
	aprilparticle::Affectors::Attractor* attractionPointTop = new aprilparticle::Affectors::Attractor(gvec3(10.0f, 0.0f, 10.0f), 6.0f);
	quazar->registerAffector(attractionPointTop);
	jetTopEmitter->addAffector(attractionPointTop);
	aprilparticle::Affectors::LinearDirectionalForce* bottom = new aprilparticle::Affectors::LinearDirectionalForce(gvec3(-1.0f, 0.0f, 0.0f), 0.024f);
	quazar->registerAffector(bottom);
	jetBottomEmitter->addAffector(bottom);
	aprilparticle::Affectors::Attractor* attractionPointBottom = new aprilparticle::Affectors::Attractor(gvec3(-10.0f, 0.0f, 10.0f), 6.0f);
	quazar->registerAffector(attractionPointBottom);
	jetBottomEmitter->addAffector(attractionPointBottom);
	aprilparticle::Affectors::Attractor* inverseGravity = new aprilparticle::Affectors::Attractor(gvec3(0.0f, 0.0f, 10.0f), -4.0f);
	quazar->registerAffector(inverseGravity);
	jetTopEmitter->addAffector(inverseGravity); // affector is shared among several emitters
	jetBottomEmitter->addAffector(inverseGravity); // affector is shared among several emitters

	quazar->setPosition(0.0f, 0.0f, 10.0f);

	discEmitter->setType(aprilparticle::Emitter::HollowSphere);
	discEmitter->setDimensions(1.0f, 6.0f, 6.0f);
	discEmitter->setBlendMode(april::ADD);
	discEmitter->setEmissionRate(240.0f);
	discEmitter->setLimit(960);
	discEmitter->setLife(4.0f);
	discEmitter->setScale(0.3f);
	
	jetTopEmitter->setType(aprilparticle::Emitter::Sphere);
	jetTopEmitter->setDimensions(1.0f, 1.0f, 1.0f);
	jetTopEmitter->setBlendMode(april::ADD);
	jetTopEmitter->setEmissionRate(120.0f);
	jetTopEmitter->setLimit(480);
	jetTopEmitter->setLife(4.0f);
	jetTopEmitter->setScale(0.3f);
	
	jetBottomEmitter->setType(aprilparticle::Emitter::Sphere);
	jetBottomEmitter->setDimensions(1.0f, 1.0f, 1.0f);
	jetBottomEmitter->setBlendMode(april::ADD);
	jetBottomEmitter->setEmissionRate(120.0f);
	jetBottomEmitter->setLimit(480);
	jetBottomEmitter->setLife(4.0f);
	jetBottomEmitter->setScale(0.3f);
}

void setupTwirl()
{
	twirl = new aprilparticle::System();
	// emitters
	aprilparticle::Emitter* left = new aprilparticle::Emitter();
	twirl->registerEmitter(left);
	aprilparticle::Emitter* right = new aprilparticle::Emitter();
	twirl->registerEmitter(right);
	// textures
	left->setTexture(redParticle); // texture is shared among several particle systems
	right->setTexture(greenParticle); // texture is shared among several particle systems
	// affectors
	aprilparticle::Affectors::LinearDirectionalForce* wind = new aprilparticle::Affectors::LinearDirectionalForce(gvec3(-1.0f, 0.0f, 0.0f), 0.012f);
	twirl->registerAffector(wind);
	left->addAffector(wind); // affector is shared among several emitters
	right->addAffector(wind); // affector is shared among several emitters
	aprilparticle::Affectors::Attractor* gravity = new aprilparticle::Affectors::Attractor(gvec3(0.0f, 0.0f, -10.0f), 7.0f);
	twirl->registerAffector(gravity);
	left->addAffector(gravity); // affector is shared among several emitters
	right->addAffector(gravity); // affector is shared among several emitters
	// affectors that are being manipulated
	aprilparticle::Affectors::DirectionalForceField* force1 = new aprilparticle::Affectors::DirectionalForceField(gvec3(10.0f, 0.0f, -10.0f), gvec3(0.0f, 0.0f, 1.0f), 4.0f, AFFECTOR_FORCE_1);
	twirl->registerAffector(force1);
	left->addAffector(force1);
	aprilparticle::Affectors::DirectionalForceField* force2 = new aprilparticle::Affectors::DirectionalForceField(gvec3(10.0f, 4.0f, -10.0f), gvec3(0.0f, 0.0f, 0.0f), 4.0f, AFFECTOR_FORCE_2);
	twirl->registerAffector(force2);
	left->addAffector(force2);
	aprilparticle::Affectors::DirectionalForceField* force3 = new aprilparticle::Affectors::DirectionalForceField(gvec3(10.0f, 0.0f, -10.0f), gvec3(1.0f, 0.0f, 0.0f), 4.0f, AFFECTOR_FORCE_3);
	twirl->registerAffector(force3);
	right->addAffector(force3);
	aprilparticle::Affectors::DirectionalForceField* force4 = new aprilparticle::Affectors::DirectionalForceField(gvec3(10.0f, 4.0f, -10.0f), gvec3(0.0f, 0.0f, 0.0f), 4.0f, AFFECTOR_FORCE_4);
	twirl->registerAffector(force4);
	right->addAffector(force4);

	twirl->setPosition(10.0f, 0.0f, -10.0f);

	left->setType(aprilparticle::Emitter::Point);
	left->setBlendMode(april::ADD);
	left->setEmissionRate(100.0f);
	left->setLimit(600);
	left->setLife(6.0f);
	left->setScaleRange(0.1f, 0.4f);
	
	right->setType(aprilparticle::Emitter::Point);
	right->setBlendMode(april::ADD);
	right->setEmissionRate(100.0f);
	right->setLimit(600);
	right->setLife(6.0f);
	right->setScaleRange(0.1f, 0.4f);
}

void april_init(const harray<hstr>& args)
{
#ifdef __APPLE__
	// On MacOSX, the current working directory is not set by
	// the Finder, since you are expected to use Core Foundation
	// or ObjC APIs to find files. 
	// So, when porting you probably want to set the current working
	// directory to something sane (e.g. .../Resources/ in the app
	// bundle).
	// In this case, we set it to parent of the .app bundle.
	{	// curly braces in order to localize variables 

		CFURLRef url = CFBundleCopyBundleURL(CFBundleGetMainBundle());
		CFStringRef path = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);
		
		// let's hope chdir() will be happy with utf8 encoding
		const char* cpath = CFStringGetCStringPtr(path, kCFStringEncodingUTF8);
		char* cpath_alloc = 0;
		if (!cpath)
		{
			// CFStringGetCStringPtr is allowed to return NULL. bummer.
			// we need to use CFStringGetCString instead.
			cpath_alloc = (char*)malloc(CFStringGetLength(path) + 1);
			CFStringGetCString(path, cpath_alloc, CFStringGetLength(path) + 1, kCFStringEncodingUTF8);
		}
		else
		{
			// even though it didn't return NULL, we still want to slice off bundle name.
			cpath_alloc = (char*)malloc(CFStringGetLength(path) + 1);
			strcpy(cpath_alloc, cpath);
		}
		// just in case / is appended to .app path for some reason
		if (cpath_alloc[CFStringGetLength(path) - 1] == '/')
		{
			cpath_alloc[CFStringGetLength(path) - 1] = 0;
		}
		
		// replace pre-.app / with a null character, thus
		// cutting off .app's name and getting parent of .app.
		strrchr(cpath_alloc, '/')[0] = 0;
							   
		// change current dir using posix api
		chdir(cpath_alloc);
		
		free(cpath_alloc); // even if null, still ok
		CFRelease(path);
		CFRelease(url);
	}
#endif
	april::init();
	april::createRenderSystem("");
	april::createRenderTarget((int)screen.x, (int)screen.y, false, "AprilParticle Demo");
	aprilparticle::init();
	april::rendersys->getWindow()->setUpdateCallback(render);
	setupGrid(2.0f);
	// textures used by more than one system
	redParticle = april::rendersys->loadTexture("../media/red_particle.png");
	greenParticle = april::rendersys->loadTexture("../media/green_particle.png");
	// setting up every system
	setupFlame();
	setupBubbles();
	setupVortex();
	setupQuazar();
	setupRain();
	setupTwirl();
}

void april_destroy()
{
	delete flame;
	delete bubbles;
	delete vortex;
	delete rain;
	delete quazar;
	delete twirl;
	delete redParticle;
	delete greenParticle;
	aprilparticle::destroy();
	april::destroy();
}

