#pragma once
#include "pch.h"

using cv::Point;
using cv::Rect;
using cv::Mat;

struct Recti {
    Recti(Point center, int width, int height) : center(center), width(width), height(height) {
        this->west = this->center.x - this->width;
        this->east = this->center.x + this->width;
        this->north = this->center.y - this->height;
        this->south = this->center.y + this->height;
    }

    bool contains(Point p) {
        return p.x >= west && p.x < east&& p.y >= north && p.y < south;
    }

    Point center;
    int width;
    int height;
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
    int offset;
public:
    bool div;
    Qt(Recti boundary, int cap, int offset) : boundary(boundary), cap(cap), offset(offset) {
        this->div = false;
        this->scalar = { 0, 0, 0 };
    }
    void sub() {
        int x = boundary.center.x;
        int y = boundary.center.y;
        int cx = boundary.width / 2;
        int cy = boundary.height / 2;

        Recti nw({ x - cx, y - cy }, cx, cy);
        root[0] = new Qt(nw, cap, offset);
        Recti ne({ x + cx, y - cy }, cx, cy);
        root[1] = new Qt(ne, cap, offset);
        Recti sw({ x - cx, y + cy }, cx, cy);
        root[2] = new Qt(sw, cap, offset);
        Recti se({ x + cx, y + cy }, cx, cy);
        root[3] = new Qt(se, cap, offset);
        div = true;
    }

    bool insert(const Point& point, int cap) {
        cap--;

        if (!boundary.contains(point)) {
            return false;
        }

        if (points.size() < cap) { //TODO: о─пр
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

    void draw(Mat& mat, std::vector<Mat>& src, int lv) {
        static Mat img;
        src[lv].copyTo(img);
        if (lv <= 0) {
            Point start = { boundary.west - offset, boundary.north - offset };
            Point end = { boundary.east + offset, boundary.south + offset };
            rectangle(mat, start, end, scalar, -1);
            return;
        }
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