//
// Created by dominik on 11.04.19.
//

#ifndef GAMEENGINE_SKELETALANIMATION_H
#define GAMEENGINE_SKELETALANIMATION_H

#include "Skeleton.h"

#include <vector>

namespace Skeleton {
/* Each bone Animation stores transformation info for each keyframe (instead of keyframes storing transformation
 * for each bone). That's the way assimp stores it's animations so I decided it's best not to change the format as
 * it doesn't really matter much.
 * */
    class boneAnimation {
    public:
    private:
        Bone* corespondingBone;
        std::vector<glm::vec3> postions;
        std::vector<glm::quat> rotation;
        std::vector<glm::vec3> scalings;
    };

    class SkeletalAnimation {
    public:
    private:
        std::string animationName;
        double durationInTicks;
        double ticksPerSecond;
        std::vector<boneAnimation> boneAnimations;
    };
}

#endif //GAMEENGINE_SKELETALANIMATION_H
