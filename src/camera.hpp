#pragma once

#include "hittable.hpp"
#include "rtx.hpp"

class Camera {
public:
    double aspect_ratio   = 1.0;
    int image_width       = 100;
    int samples_per_pixel = 10;

    void render(const Hittable &world) {
        initialize();

        std::cout
                << "P3\n"
                << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; ++j) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                Color pixelColor(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel; ++sample) {
                    Rayd r = getRay(i, j);
                    pixelColor += rayColor(r, world);
                }
                writeColor(std::cout, pixel_samples_scale * pixelColor);
            }
        }

        std::clog << "\rDone.                        \n";
    }

private:
    int image_height;
    double pixel_samples_scale;
    Point3d center;
    Point3d pixel00_loc;
    Point3d pixel_delta_u;
    Point3d pixel_delta_v;

    void initialize() {
        image_height        = std::max(1, int(image_width / aspect_ratio));
        center              = Point3d(0, 0, 0);
        pixel_samples_scale = 1.0 / samples_per_pixel;

        auto focal_length    = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width  = viewport_height * (double(image_width) / image_height);

        auto viewport_u = Vec3d(viewport_width, 0, 0);
        auto viewport_v = Vec3d(0, -viewport_height, 0);
        pixel_delta_u   = viewport_u / image_width;
        pixel_delta_v   = viewport_v / image_height;

        auto viewport_upper_left = center - Vec3d(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc              = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    [[nodiscard]] static Vec3d sampleSquare() {
        return {randomDouble() - 0.5, randomDouble(), 0};
    }

    // Constructs a camera ray from origin directed at randomly sampled point around (i, j)
    [[nodiscard]] Rayd getRay(int i, int j) const {
        auto offset      = sampleSquare();
        auto pixelSample = pixel00_loc + ((i + offset.x) * pixel_delta_u) + ((j + offset.y) * pixel_delta_v);
        return {center, pixelSample - center};
    }

    static Color rayColor(const Rayd &r, const Hittable &world) {
        HitRecord rec;
        if (world.hit(r, Interval(0, INFINITY_D), rec)) {
            return 0.5 * (rec.normal + Color(1, 1, 1));
        }

        auto unit = normalize(r.dir);
        auto a    = 0.5 * (unit.y + 1.0);
        return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
    }
};
