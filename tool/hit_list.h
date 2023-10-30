#ifndef _HITLIST_H_
#define _HITLIST_H_

#include"object.h"
#include<memory>
#include<vector>
#include"tools.h"

class hit_list : public object{
public:
    std::vector<std::shared_ptr<object>> objects;
public:
    hit_list() = default;
    hit_list(const std::shared_ptr<object> &obj) { add(obj); }

    void clear() { objects.clear(); }
    void add(std::shared_ptr<object> obj) { objects.push_back(obj); }

    virtual bool hit(const ray &r, const double& tmin, const double& tmax, hit_record &rec) const;
    virtual std::shared_ptr<bounding_box> get_aabb() const;
    
};

bool hit_list::hit(const ray &r, const double& tmin, const double& tmax, hit_record &rec) const{
    hit_record t_rec;
    bool hits = false;
    double tt = tmax;
    for(auto obj : objects){
        if( obj->hit(r,tmin,tt,t_rec)){
            hits = true;
            tt = t_rec.t;
            rec = t_rec;
        }
    }
    return hits;
}

inline std::shared_ptr<bounding_box> hit_list::get_aabb() const
{
    bounding_box kk;
    bool first_aabb = true;
    for(auto obj : objects){
        auto k = obj->get_aabb();
        if( !k )
            return nullptr;
        kk = first_aabb ? *k : surrounding_box(*k, kk);
        first_aabb = false;
    }
    return std::make_shared<bounding_box>(kk);
}

#endif  
