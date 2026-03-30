#ifndef _ANIMATION_KEYFRAMES
#define _ANIMATION_KEYFRAMES


#include <vector>
#include <glm/glm.hpp>

using namespace std;


// AnimKeyframes contains all information related to a single animation.
// These are the animation speed measured by millisecsPerKeyframe
// and texture coordinates for all keyframes.


struct AnimKeyframes
{
	float millisecsPerKeyframe = 0.f;
	vector<glm::vec2> keyframeDispl = vector<glm::vec2>();
};


#endif // _ANIMATION_KEYFRAMES


