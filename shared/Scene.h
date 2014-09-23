#ifndef scene_h
#define scene_h

typedef void (*SceneCallback)(int cue);

typedef struct {
	int numCues;
	SceneCallback callBack;
} Scene;

#endif
