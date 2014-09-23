#ifndef showplot_h
#define showplot_h

// Number of cues in each scene
// FIXME: these must be manually maintained
const Scene SCENE_TEST			= {1,sceneTest};
const Scene SCENE_WHITE_FLASHES = {1,sceneWhiteFlashes};
const Scene SCENE_WHITE_BREATH	= {1,sceneWhiteBreath};
const Scene SCENE_COLOUR		= {1,sceneColour};
const Scene SCENE_MAGNETOMETER	= {1,sceneMagnetometer};
const Scene SCENE_FINALE		= {1,sceneFinale};

// Sequence of scenes in show
const Scene SHOW_PLOT[] = {
	SCENE_TEST,
	SCENE_WHITE_FLASHES,
	SCENE_WHITE_BREATH,
	SCENE_COLOUR,
	SCENE_MAGNETOMETER,
	SCENE_FINALE
};

const short NUM_SCENES_IN_SHOW_PLOT = sizeof(SHOW_PLOT) / sizeof(Scene);

#endif