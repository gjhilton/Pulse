#ifndef showplot_h
#define showplot_h

// Number of cues in each scene
// FIXME: these must be manually maintained
const Scene SCENE_TEST			= {2,sceneTest};
const Scene SCENE_WHITE_FLASHES = {5,sceneWhiteFlashes};
const Scene SCENE_WHITE_BREATH	= {1,sceneWhiteBreath};
const Scene SCENE_CHASE			= {7,sceneChase};
const Scene SCENE_COLOUR		= {9,sceneColour};
const Scene SCENE_MAGNETOMETER	= {1,sceneMagnetometer};

// Sequence of scenes in show
const Scene SHOW_PLOT[] = {
	SCENE_TEST,
	SCENE_WHITE_FLASHES,
	SCENE_WHITE_BREATH,
	SCENE_CHASE,
	SCENE_COLOUR,
	SCENE_MAGNETOMETER
};

const short NUM_SCENES_IN_SHOW_PLOT = sizeof(SHOW_PLOT) / sizeof(Scene);

#endif