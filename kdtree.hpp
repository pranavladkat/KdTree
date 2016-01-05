#ifndef KDTREE_HPP
#define KDTREE_HPP

#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <memory>
#include <limits>
#include <cassert>


template<typename Point>
class KdTree{
public:
    KdTree():root(nullptr){}
    ~KdTree(){}

    void build_kdtree(std::vector<std::shared_ptr<Point>>& points){
        root = build_subtree(points);
    }

    size_t nearest_neighbour(const Point& qpoint) const {
        // pair of (id, distance) representing nearest node
        std::pair<size_t, double> best_node(0,std::numeric_limits<double>::max());
        //start with root
        size_t id = nearest(qpoint, root, best_node);
        return id;
    }


private:
    struct KdNode{
        std::shared_ptr<Point> point;                        // point data
        std::pair<size_t, typename Point::value_type> split; // split(dimension,value)
        bool leaf;                                           // is it a leaf node?
        std::shared_ptr<KdNode> kd_left;                     // left child
        std::shared_ptr<KdNode> kd_right;                    // right child
        KdNode(): leaf(false), kd_left(nullptr), kd_right(nullptr) {}
    };

    std::shared_ptr<KdNode> root;   // root node

    std::shared_ptr<KdNode> build_subtree(std::vector<std::shared_ptr<Point>>& points){

        // return null if empty input vector
        if(points.size() == 0){
            return nullptr;
        }

        std::shared_ptr<KdNode> node = std::make_shared<KdNode>();

        // leaf node if only 1 element in input vector
        if(points.size() == 1){
            node->point = points[0];
            node->leaf = true;
            return node;
        }

        // get split dimension and value (also partitions input vector about median)
        // i.e. all elements less than split value are at < median position
        //      and elements greater than split value are at > median position
        node->split = get_split(points);

        size_t median = points.size() / 2;

        // assign point at median to current node
        node->point = points[median];

        // create vector of points which are less/greater than key value
        std::vector<std::shared_ptr<Point>> left_vec(points.begin(), points.begin()+median);
        std::vector<std::shared_ptr<Point>> right_vec(points.begin()+median+1, points.end());

        // build left/right subtree
        node->kd_left = build_subtree(left_vec);
        node->kd_right = build_subtree(right_vec);

        return node;
    }

    std::pair<size_t, typename Point::value_type> get_split(std::vector<std::shared_ptr<Point>>& points){

        std::pair<size_t, typename Point::value_type> split(-1,-1);
        size_t total_points = points.size();
        size_t dim = points[0]->data.size();
        double max_var = -1;

        // get dimension with maximum variance
        for(size_t i = 0; i < dim; ++i){
            double mean = 0, var = 0, x;
            // calculate mean
            for(size_t j = 0; j < total_points; ++j){
                mean += points[j]->data[i];
            }
            mean /= (double)total_points;

            // calculate variance
            for(size_t j = 0; j < total_points; ++j){
                x = points[j]->data[i] - mean;
                var += x * x;
            }
            var /= ((double)total_points-1);

            // get dimension with max variance
            if(max_var < var){
                split.first = i;
                max_var = var;
            }
        }

        size_t median = total_points / 2;

        // find median in the dimension having max variance
        std::nth_element(points.begin(), points.begin()+median, points.end(),
                         [&split](std::shared_ptr<Point> a, std::shared_ptr<Point> b){
            return a->data[split.first] < b->data[split.first];
        });

        // get split value
        split.second = points[median]->data[split.first];

        return split;
    }

    // find nearest node
    size_t nearest(const Point& qpoint, const std::shared_ptr<KdNode> node, std::pair<size_t,double>& best) const {

        //std::cout << "best = " << best.first << " - " << best.second << std::endl;

        size_t id = node->point->id;
        double distance = this->distance(qpoint,node->point);

        if(distance < best.second){
            best.first = id;
            best.second = distance;
        }

        if(!node->leaf){

            std::shared_ptr<KdNode> near, far;
            if(qpoint.data[node->split.first] <= node->split.second){
                near = node->kd_left;
                far = node->kd_right;
            }else{
                near = node->kd_right;
                far = node->kd_left;
            }

            id = nearest(qpoint,near,best);
            if(distance >= best.second)
                return best.first;
            id = nearest(qpoint,far,best);
        }

        return best.first;
    }

    // computes euclidian distance
    double distance(const Point& pt1, const std::shared_ptr<Point> pt2) const {
        size_t dim = pt1.data.size();
        double distance = 0;
        for(size_t i = 0; i < dim; i++){
            double x = pt1.data[i] - pt2->data[i];
            distance += x*x;
        }
        return distance;
    }

}; // end class def


#endif // KDTREE_HPP

