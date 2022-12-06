#pragma once
#include "pch.h"

using cv::Point;
using cv::Rect;
using cv::Mat;

struct Recti {
    Recti(float cx, float cy, float width, float height) : cx(cx), cy(cy), width(width), height(height) {
        this->west = (this->cx - this->width);
        this->east = (this->cx + this->width);
        this->north = (this->cy - this->height);
        this->south = (this->cy + this->height);
    }

    bool contains(Point p) {
        return p.x >= west && p.x < east && p.y >= north && p.y < south;
    }

    float cx;
    float cy;
    float width;
    float height;
    int west;
    int east;
    int north;
    int south;
};

class Qt {
private:
    Recti boundary;
    Qt* root[4];
    std::queue<Point> points;
    int cap;
    cv::Scalar scalar;
public:
    bool div;

    Qt(Recti boundary, int cap) : boundary(boundary), cap(cap) {
        this->div = false;
        this->scalar = {0, 0, 0};
    }

    void sub() {
        float cx = boundary.cx;
        float cy = boundary.cy;
        float w = boundary.width / 2.0;
        float h = boundary.height / 2.0;

        Recti nw(cx - w, cy - h, w, h);
        root[0] = new Qt(nw, cap);
        Recti ne(cx + w, cy - h, w, h);
        root[1] = new Qt(ne, cap);
        Recti sw(cx - w, cy + h, w, h);
        root[2] = new Qt(sw, cap);
        Recti se(cx + w, cy + h, w, h);
        root[3] = new Qt(se, cap);
        div = true;
    }

    bool insert(const Point& point, int cap) {
        cap--;

        if (!boundary.contains(point)) {
            return false;
        }

        if (points.size() < cap) {
            points.push(point);
            return true;
        }

        if (!div) {
            sub();
        }

        if (root[0]->insert(point, cap))
            return true;
        if (root[1]->insert(point, cap))
            return true;
        if (root[2]->insert(point, cap))
            return true;
        if (root[3]->insert(point, cap))
            return true;
        return false;
    }

    void draw(Mat& mat, vector<Mat>& src, int lv) {
        static Mat img;
        src[lv].copyTo(img);
        Rect roi(Point(boundary.west, boundary.north), img.size());
        Mat insetImage(mat, roi);
        if (lv <= 3) {
            img -= cv::Scalar(255 / (lv + 1), 255 / (lv + 1), 255 / (lv + 1));
        }
        img.copyTo(insetImage);
    }

    void show(Mat& mat, Mat& img2, int lv, bool is_static_img) {
        static bool first_run = true;
        static bool first_frame = true;
        static std::vector<Mat> img(this->cap + 1);
        if (first_run || (first_frame && !is_static_img)) {
            cv::resize(img2, img2, mat.size(), 0, 0, cv::INTER_CUBIC);
            float scaling = 1;
            for (int i = 0; i < this->cap + 1; i++) {
                cv::resize(img2, img[this->cap - i], cv::Size(), scaling, scaling, cv::INTER_CUBIC);
                scaling *= 0.5;
            }
            first_run = false;
        }

        this->draw(mat, img, lv);
        if (div) {
            root[0]->show(mat, img2, lv - 1, is_static_img);
            root[1]->show(mat, img2, lv - 1, is_static_img);
            root[2]->show(mat, img2, lv - 1, is_static_img);
            root[3]->show(mat, img2, lv - 1, is_static_img);
            first_frame = (lv == 7);
        }
    }

    void free() {
        for (int i = 0; i < 4; i++) {
            delete root[i];
        }
    }

    ~Qt() {
        free();
    }
};
