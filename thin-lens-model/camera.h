#ifndef CAMERA_H
#define CAMERA_H
#include "vec3.h"
#include "ray.h"
#include "sampler.h"
class Camera {
    public:
        Vec3 camPos; //カメラの位置
        Vec3 camForward; //カメラの前方向
        Vec3 camRight; //カメラの右方向
        Vec3 camUp; //カメラの上方向

        Camera(const Vec3& camPos, const Vec3& camForward) : camPos(camPos), camForward(camForward) {
            camRight = -1 * normalize(cross(camForward, Vec3(0, 1, 0)));
            camUp = normalize(cross(camRight, camForward));
        };

        //画素(u, v)に対応するレイを計算する
        virtual Ray getRay(double u, double v, double &w) const = 0;
};


//ピンホールカメラ
class PinholeCamera : public Camera {
    public:
        PinholeCamera(const Vec3& camPos, const Vec3& camForward) : Camera(camPos, camForward) {};

        Ray getRay(double u, double v, double &w) const {
            //像が逆になるので修正
            u = -u;
            v = -v;
            //ピンホールの位置
            Vec3 pinhole = camPos + camForward;
            //注目しているセンサーの位置
            Vec3 sensorPos = camPos + u*camRight + v*camUp;
            //レイの方向
            Vec3 rayDir = normalize(pinhole - sensorPos);
            //寄与係数の計算
            w = std::pow(dot(camForward, rayDir), 4.0);
            return Ray(camPos, rayDir);
        };
};


//薄レンズモデルカメラ
class ThinLensCamera : public Camera {
    public:
        //センサー面からレンズ面までの距離
        double lensDistance;
        //フォーカルを合わせる位置
        Vec3 focusPoint;
        //主点からオブジェクトプレーンまでの距離
        double objectDistance;
        //焦点距離
        double focalLength;
        //F値
        double Fnumber;
        //レンズの半径
        double lensRadius;
        //レンズの中心位置
        Vec3 lensCenterPos;

        ThinLensCamera(const Vec3& camPos, const Vec3& camForward, double lensDistance, const Vec3& focusPoint, double Fnumber) : Camera(camPos, camForward), lensDistance(lensDistance), focusPoint(focusPoint), Fnumber(Fnumber) {
            objectDistance = (focusPoint - camPos).length() - lensDistance;
            focalLength = 1.0/(1.0/lensDistance + 1.0/objectDistance);
            lensRadius = 0.5 * focalLength/Fnumber;
            lensCenterPos = camPos + lensDistance*camForward;
        };

        Ray getRay(double u, double v, double &w) const {
            //像が逆になるので修正
            u = -u;
            v = -v;
            //センサー面上の位置
            Vec3 sensorPos = camPos + u*camRight + v*camUp;
            //レンズ上の位置をサンプリング
            Vec3 l = sampleDisk();
            Vec3 lensPos = lensCenterPos + lensRadius*(l.x*camRight + l.y*camUp);

            //センサー面からレンズ中心までの方向ベクトル
            Vec3 sensorToLensCenter = normalize(lensCenterPos - sensorPos);
            //センサー面からオブジェクト面までの距離
            double sensorObjectDistance = (lensDistance + objectDistance)/dot(camForward, sensorToLensCenter);
            //オブジェクト面での位置
            Vec3 objectPos = sensorPos + sensorObjectDistance*sensorToLensCenter;

            //センサー面からレンズ面までの方向ベクトル
            Vec3 sensorToLensPos = normalize(lensPos - sensorPos);
            //センサー面からレンズ面までの距離の二乗
            double sensorLensDistance2 = (lensPos - sensorPos).length2();
            //寄与係数の計算
            w = lensDistance*lensDistance*std::pow(dot(camForward, sensorToLensPos), 2.0)/sensorLensDistance2;

            Vec3 rayDir = normalize(objectPos - lensPos);
            return Ray(lensPos, rayDir);
        };
};
#endif
