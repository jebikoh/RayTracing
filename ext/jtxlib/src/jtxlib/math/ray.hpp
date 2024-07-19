#pragma once

#include <jtxlib/math/numerical.hpp>
#include <jtxlib/math/vec3.hpp>
#include <jtxlib/math/vecmath.hpp>

namespace jtx {

    JTX_NUM_ONLY_T
    class Ray {
    public:
        Point3<T> origin;
        Vec3<T> dir;
        T time;
        // Add medium later

        [[nodiscard]] inline bool valid() const {
            return origin.valid() && dir.valid() && !jtx::isNaN(time);
        }

        //region Constructors
        Ray() : origin(), dir(), time(JTX_ZERO) {}

        Ray(const Point3<T> &origin, const Vec3<T> &direction, T time = JTX_ZERO)
            : origin(origin), dir(direction), time(time) {
            ASSERT(valid());
        }

        Ray(const Ray &other) : origin(other.origin), dir(other.dir), time(other.time) {
            ASSERT(valid());
        }

        ~Ray() = default;
        //endregion

        [[nodiscard]] inline Point3<T> at(T t) const {
            return origin + t * dir;
        }
    };

    JTX_NUM_ONLY_T
    class RayDifferential : public Ray<T> {
    public:
        Point3<T> rxOrigin, ryOrigin;
        Vec3<T> rxDirection, ryDirection;
        bool hasDiffs;

        [[nodiscard]] inline bool valid() const {
            return Ray<T>::valid() &&
                   (!hasDiffs || (rxOrigin.valid() && ryOrigin.valid() && rxDirection.valid() && ryDirection.valid()));
        }

        //region Constructors
        RayDifferential() : Ray<T>(), rxOrigin(), ryOrigin(), rxDirection(), ryDirection(), hasDiffs(false) {}

        RayDifferential(const Point3<T> &origin, const Vec3<T> &direction, T time = JTX_ZERO)
            : Ray<T>(origin, direction, time), rxOrigin(), ryOrigin(), rxDirection(), ryDirection(), hasDiffs(false) {
            ASSERT(valid());
        }

        explicit RayDifferential(const Ray<T> &ray) : Ray<T>(ray), rxOrigin(), ryOrigin(), rxDirection(), ryDirection(),
                                                      hasDiffs(false) {
            ASSERT(valid());
        }

        RayDifferential(const RayDifferential &other) : Ray<T>(other), rxOrigin(other.rxOrigin), ryOrigin(other.ryOrigin),
                                                        rxDirection(other.rxDirection), ryDirection(other.ryDirection),
                                                        hasDiffs(other.hasDiffs) {
            ASSERT(valid());
        }
        //endregion

        inline void scale(T s) {
            rxOrigin = this->origin + (rxOrigin - this->origin) * s;
            ryOrigin = this->origin + (ryOrigin - this->origin) * s;
            rxDirection = this->dir + (rxDirection - this->dir) * s;
            ryDirection = this->dir + (ryDirection - this->dir) * s;
        }
    };

    [[maybe_unused]] typedef Ray<float> Rayf;
    [[maybe_unused]] typedef Ray<double> Rayd;
}// namespace jtx
