#ifndef _BVH_H_
#define _BVH_H_

#include"../tool/tools.h"
#include "../tool/object.h"
#include<vector>
#include<algorithm>

class bvh : public object{
public:
    std::shared_ptr<object> left, right;
    bounding_box box;

public:
    bvh() = default;
    //这里传入的都是物体
    bvh(std::vector<std::shared_ptr<object>> &objects, 
        const size_t& start, const size_t& end);
    virtual bool hit(const ray &r, const double &tmin, const double &tmax, hit_record &rec) const;
    virtual std::shared_ptr<bounding_box> get_aabb() const;
};

std::shared_ptr<bounding_box> bvh::get_aabb() const{
    return std::make_shared<bounding_box>(box);
}

inline bvh::bvh(std::vector<std::shared_ptr<object>> &objects, const size_t& start, const size_t& end)
{
    int k  = random_int(0,2) ;
    auto cmp = k == 0 ? box_x_compare : k == 1 ? box_y_compare
                                               : box_z_compare;
    size_t object_span = end - start;

    if( object_span == 1 ) {
        left = right = objects[start]; //这里放的就是最后的一个叶子节点，存放实际物体 而不是 bvh object_span =2 时同理
    }else if( object_span == 2){
        if( cmp(objects[start], objects[end-1])){
            left = objects[start];
            right = objects[end-1];
        }else{
            right = objects[start];
            left = objects[end-1];
        }
    }else{
        std::sort(objects.begin() + start, objects.begin() + end, cmp);
        auto mid = start + object_span / 2;
        left = std::make_shared<bvh>(objects, start, mid);
        right = std::make_shared<bvh>(objects, mid, end);
    }
    auto k1 = left->get_aabb(), k2 = right->get_aabb();
    if( !k1 || !k2 ){
        std::cerr << "No bounding box can be found" << std::endl;
    }
    box = surrounding_box(k1, k2); // 得到更大的包围盒
}   

bool bvh::hit(const ray &r, const double &tmin, const double &tmax, hit_record &rec) const
{
    if( !box.hit(r) ){
        return false;
    }
    auto k1 = left->hit(r, tmin, tmax, rec);
    auto k2 = right->hit(r, tmin, k1 ? rec.t : tmax, rec);
    return left->hit(r, tmin, tmax, rec) || right->hit(r, tmin, tmax, rec);
    // left 可以指向不同的数据类型， 只有到最后的叶节点才会进行 与实物相交
}

#endif