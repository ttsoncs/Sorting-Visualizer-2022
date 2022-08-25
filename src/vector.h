#ifndef VECTOR_H
#define VECTOR_H

#include "../lib/raylib-4.2.0/src/raylib.h"
#include "visualize.h"
#include <algorithm>
#include <random>
#include <vector>

class Vector {
  public:
    Vector() {
        Vector::generate();
        Vector::randomShuffle(false);
    }

    static void generate() {
        Vector::v_.clear();
        Vector::v_.reserve(Vector::v_size_);
        auto [r1, g1, b1, a1] = Vector::getRandomColor();
        auto [r2, g2, b2, a2] = Vector::getRandomColor();
        auto deltaRed{static_cast<float>(r2 - r1) / static_cast<float>(Vector::v_size_)};
        auto deltaGreen{static_cast<float>(g2 - g1) / static_cast<float>(Vector::v_size_)};
        auto deltaBlue{static_cast<float>(b2 - b1) / static_cast<float>(Vector::v_size_)};
        auto deltaHeight{static_cast<float>(GetScreenHeight()) / static_cast<float>(Vector::v_size_)};
        auto barHeight{0.0f};
        for (auto i{0}; i != Vector::v_size_; ++i) {
            barHeight += deltaHeight;
            v_.emplace_back(std::make_pair(i, std::make_pair(barHeight, Color{
                                                                            static_cast<unsigned char>(r1 + i * deltaRed),
                                                                            static_cast<unsigned char>(g1 + i * deltaGreen),
                                                                            static_cast<unsigned char>(b1 + i * deltaBlue),
                                                                            255})));
        }
    }

    static auto randomShuffle(bool isConstructed = true) -> void {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(Vector::v_.begin(), Vector::v_.end(), g);
        if (isConstructed) {
            Window::beginDrawing();
            Visualize::visualizeVector(Vector::v_);
            Visualize::visualizeTitle("Random Shuffle");
            Window::endDrawing();
        }
    }

    static auto reverseSortedShuffle(bool isConstructed = true) -> void {
        std::sort(Vector::v_.begin(), Vector::v_.end(), [](auto const &a, auto const &b) {
            return a.first > b.first;
        });
        if (isConstructed) {
            Window::beginDrawing();
            Visualize::visualizeVector(Vector::v_);
            Visualize::visualizeTitle("Reverse Sorted Shuffle");
            Window::endDrawing();
        }
    }

    static auto almostSortedShuffle(bool isConstructed = true) -> void {
        std::sort(Vector::v_.begin(), Vector::v_.end(), [](auto const &a, auto const &b) {
            return a.first < b.first;
        });
        std::random_device rd;
        std::mt19937 g(rd());
        std::uniform_int_distribution<int> dSize((Vector::v_size_ / 4), (Vector::v_size_ / 2));
        std::uniform_int_distribution<int> dIndex(0, Vector::v_size_ - 1);
        auto size{dSize(g)};
        for (auto i{0}; i != size; ++i) {
            auto firstIndex{dIndex(g)};
            auto secondIndex{dIndex(g)};
            std::swap(v_[firstIndex], v_[secondIndex]);
        }
        if (isConstructed) {
            Window::beginDrawing();
            Visualize::visualizeVector(Vector::v_);
            Visualize::visualizeTitle("Almost Sorted Shuffle");
            Window::endDrawing();
        }
    }

    static auto getVector() { return Vector::v_; }

    static auto getVectorSize() { return Vector::v_size_; }

    static auto incrementVectorSize() {
        if (Vector::v_size_ < 480) {
            Vector::v_size_ += 5;
        }
        Vector::generate();
        Vector::randomShuffle(false);
        Visualize::visualizeVector(Vector::getVector());
        Visualize::visualizeTitle("Increment Vector Size");
    }

    static auto decrementVectorSize() {
        if (Vector::v_size_ > 10) {
            Vector::v_size_ -= 5;
        }
        Vector::generate();
        Vector::randomShuffle(false);
        Visualize::visualizeVector(Vector::getVector());
        Visualize::visualizeTitle("Decrement Vector Size");
    }

  private:
    inline static std::vector<std::pair<int, std::pair<float, Color>>> v_;
    inline static auto v_size_{120};

    static Color getRandomColor() {
        std::random_device rd;
        std::mt19937 g(rd());
        std::uniform_int_distribution<int> rgb(0, 255);
        return {static_cast<unsigned char>(rgb(g)), static_cast<unsigned char>(rgb(g)), static_cast<unsigned char>(rgb(g)), 255};
    }
};

#endif