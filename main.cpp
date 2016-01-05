#include <iostream>
#include <vector>
#include <memory>
#include <cassert>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "kdtree.hpp"

// struct to define point data
template <typename DATATYPE>
struct Point{
    size_t id;                      // id stores the original line number (0-based index)
    std::vector<DATATYPE> data;     // point data, k-dimensional vector
    using value_type = DATATYPE;    // to refer data type later
};

template <typename T>
void read_data(const std::string& filename, std::vector<std::shared_ptr<T>>& data);

template <typename T>
void print_data(const std::vector<std::shared_ptr<T>>& data);


int main()
{

    // input data is stored as vector of pointers
    std::vector<std::shared_ptr<Point<double>>> data;

    std::string name;
    std::cout << "Enter filename: ";
    std::cin >> name;

    read_data(name, data);
    print_data(data);

    KdTree<Point<double>> kdtree;

    kdtree.build_kdtree(data);

    Point<double> p;
    std::string point;
    std::cout << "Enter point: ";
    std::cin.ignore();
    std::getline(std::cin, point);
    std::istringstream value(point);
    double temp;
    while(value >> temp)
        p.data.push_back(temp);

    std::cout << "nearest node = " << kdtree.nearest_neighbour(p) << std::endl;

    return 0;
}


// read data from file
template <typename T>
void read_data(const std::string& filename, std::vector<std::shared_ptr<T>>& data){

    std::ifstream file(filename);
    assert(file.is_open());

    std::string line;
    typename T::value_type tmp;
    size_t id = 0;

    // assuming each point is on separate line
    // this reads 1 line from a file at a time...usefull for k-dimensional data
    while(std::getline(file, line)){
        std::shared_ptr<T> point = std::make_shared<T>();
        point->id = id;
        std::istringstream value(line);
        while(value >> tmp){
            point->data.push_back(tmp);
        }
        data.push_back(std::move(point));
        id++;
    }
}

template <typename T>
void print_data(const std::vector<std::shared_ptr<T>>& data) {

    std::for_each(data.begin(),data.end(),[](const std::shared_ptr<T> p){
        std::cout << p->id << " : ";
        std::for_each(p->data.begin(),p->data.end(),[](const typename T::value_type& v){
            std::cout << v << " ";
        });
        std::cout << std::endl;
    });
}
