#include "pch.h"
#include "model.h"

//namespace model
//{
//    Geometry humanoid_geometry = { {
//        {
//            L"hat", L"head", {0, 38, 0},
//            {
//                {{-4, 37.5, -4}, {8, 8, 8}, {0, 16}},
//            }, true
//        },
//
//        {
//            L"head", L"body", {0, 24, 0},
//            {
//                {{-4, 24, -4}, {8, 8, 8}, {0, 0}},
//            }, true
//        },
//
//        {
//            L"body", L"no parent", {0, 38, 0},
//            {
//                {{-4, 26, -2}, {8, 12, 4}, {32, 16}},
//            }, true
//        },
//
//        {
//            L"rightArm", L"body", {-3, 36, 0},
//            {
//                {{-6, 8, -1}, {2, 30, 2}, {56, 0}},
//            }, true
//        },
//
//        {
//            L"rightItem", L"rightArm", {-6, 15, 1},
//            {
//                {{0, 0, 0}, {1, 1, 1}, {1, 1}},
//            }, false
//        },
//
//        {
//            L"leftArm", L"body", {5, 36, 0},
//            {
//                {{4, 8, -1}, {2, 30, 2}, {56, 0}},
//            }, true
//        },
//
//        {
//            L"rightLeg", L"body", {-2, 26, 0},
//            {
//                {{-3, -4, -1}, {2, 30, 2}, {56, 0}},
//            }, true
//        },
//
//        {
//            L"leftLeg", L"body", {2, 26, 0},
//            {
//                {{1, -4, -1}, {2, 30, 2}, {56, 0}},
//            }, true
//        }
//    } };
//
//}
namespace model
{
    Geometry humanoid_geometry = { {
        {
            L"body", L"no parent", {0, 24, 0},
            {
                {{-4, 12, -2}, {8, 12, 4}, {16, 16}},

            }, true
        },
        {
            L"waist", L"no parent", {0, 12, 0},
            {
                {{0, 0, 0}, {1, 1, 1}, {1, 1}},
            },false
        },
{
L"head", L"no parent", {0, 24, 0},
{
{{-4, 24, -4}, {8, 8, 8}, {0, 0}},

}, true
},
{
L"hat", L"no parent", {0, 24, 0},
{
{{-4, 24, -4}, {8, 8, 8}, {32, 0}},

}, true
},
{
L"rightArm", L"no parent", {-5, 22, 0},
{
{{-8, 12, -2}, {4, 12, 4}, {40, 16}},

}, true
},
{
L"rightItem", L"rightArm", {-6, 15, 1},
{
{{0, 0, 0}, {1, 1, 1}, {1, 1}},
},false
},
{
L"leftArm", L"no parent", {5, 22, 0},
{
{{4, 12, -2}, {4, 12, 4}, {40, 16}},

}, true
},
{
L"rightLeg", L"no parent", {-1.9, 12, 0},
{
{{-3.9, 0, -2}, {4, 12, 4}, {0, 16}},

}, true
},
{
L"leftLeg", L"no parent", {1.9, 12, 0},
{
{{-0.1, 0, -2}, {4, 12, 4}, {0, 16}},

}, true
},
{
L"helmet", L"no parent", {0, 24, 0},
{
{{0, 0, 0}, {1, 1, 1}, {1, 1}},
},false
},
{
L"rightArmArmor", L"rightArm", {-5, 22, 0},
{
{{0, 0, 0}, {1, 1, 1}, {1, 1}},
},false
},
{
L"leftArmArmor", L"leftArm", {5, 22, 0},
{
{{0, 0, 0}, {1, 1, 1}, {1, 1}},
},false
},
{
L"rightLegging", L"rightLeg", {-1.9, 12, 0},
{
{{0, 0, 0}, {1, 1, 1}, {1, 1}},
},false
},
{
L"leftLegging", L"leftLeg", {1.9, 12, 0},
{
{{0, 0, 0}, {1, 1, 1}, {1, 1}},
},false
},
{
L"rightBoot", L"rightLeg", {-1.9, 12, 0},
{
{{0, 0, 0}, {1, 1, 1}, {1, 1}},
},false
},
{
L"leftBoot", L"leftLeg", {1.9, 12, 0},
{
{{0, 0, 0}, {1, 1, 1}, {1, 1}},
},false
},
{
L"rightSock", L"rightLeg", {-1.9, 12, 0},
{
{{0, 0, 0}, {1, 1, 1}, {1, 1}},
},false
},
{
L"leftSock", L"leftLeg", {1.9, 12, 0},
{
{{0, 0, 0}, {1, 1, 1}, {1, 1}},
},false
},
    {
        L"bodyArmor", L"body", {0, 24, 0},
        {
        {{0, 0, 0}, {1, 1, 1}, {1, 1}},
        },false
    },
    {
        L"belt", L"body", {0, 24, 0},
        {
            {{0, 0, 0}, {1, 1, 1}, {1, 1}},
        },false
    }
} };
}