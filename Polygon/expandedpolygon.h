#ifndef EXPANDEDPOLYGON_H
#define EXPANDEDPOLYGON_H
#include <iostream>
#include <exception>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/algorithms/distance.hpp>
#include <boost/assign/list_of.hpp>
namespace bg = boost::geometry;
using point_t = bg::model::d2::point_xy<double>;
using polygon_t = bg::model::polygon<point_t,true,true,std::vector,std::vector,std::allocator,std::allocator>;

namespace procon { namespace polygon {

class ExpandedPolygon : public polygon_t
{
    //メンバ
    int size;
    std::vector<double> side_length;
    std::vector<double> side_angle;

    //calc
    //calcSideSize -> calcSideLength -> calcSideAngle
    void calcSize();
    void calcSideLength();
    void calcSideAngle();

public:
    //constructor
    ExpandedPolygon();
    ExpandedPolygon(ExpandedPolygon const& p);

    //getter
    int getSize();
    std::vector<double> getSideLength();
    std::vector<double> getSideAngle();

    //setter
    void setPoints(std::vector<point_t> const& p);

    //operator
    ExpandedPolygon operator =  (ExpandedPolygon const& p);

    //calcAll
    //***ポリゴン変更後必ず実行のこと***
    void updatePolygon();

    //内側の輪を強引に持ってしまう
    //**使わない**
    std::vector<bg::model::ring<point_t>*> interiors;
};

}}

//以下のコードはExPolygonをboostのもろもろで使えるようにするためのやつ
//詳しくは下記URLを参照のこと
//http://www.boost.org/doc/libs/1_54_0/libs/geometry/doc/html/geometry/examples/example_source_code__adapting_a_legacy_geometry_object_model.html

//CustomPolyRingIterator
template <class I, // Line iterator type
          class R  // Point reference type
         >
class PolyRingIterator : public boost::iterator_facade<
        PolyRingIterator<I,R>, R, std::random_access_iterator_tag, R> //new traversal tag
{
public:
    PolyRingIterator() {}

    explicit PolyRingIterator(I ringIter) : _ringIter(ringIter) {}

    template<class OtherI, class OtherR>
    PolyRingIterator(PolyRingIterator<OtherI, OtherR> const& other) :
        _ringIter(other.getRingIter()) {}

    I getRingIter() const {return _ringIter;}

    typedef typename boost::iterator_facade<PolyRingIterator<I,R>, R, std::random_access_iterator_tag, R>::difference_type difference_type;

private:
    friend class boost::iterator_core_access;

    void increment()
    {
        ++_ringIter;
    }

    void decrement()
    {
        --_ringIter;
    }

    void advance(difference_type n)
    {
        std::advance(_ringIter,n);
    }

    difference_type distance_to(const PolyRingIterator& other) const
    {
        return std::distance(_ringIter, other.getRingIter());
    }

    bool equal(const PolyRingIterator& other) const
    {
        return _ringIter == other.getRingIter();
    }

    R dereference() const {return *(*_ringIter);}

    I _ringIter;
};

//CustomPolygonRingRange
typedef PolyRingIterator<std::vector<bg::model::ring<point_t>*>::iterator, bg::model::ring<point_t>> PolygonRingIterator;
typedef PolyRingIterator<std::vector<bg::model::ring<point_t>*>::const_iterator, const bg::model::ring<point_t>> ConstPolygonRingIterator;

class CustomPolygonRingRange
{
    PolygonRingIterator _begin;
    PolygonRingIterator _end;

    bool isIterSet;

    ConstPolygonRingIterator _cbegin;
    ConstPolygonRingIterator _cend;

    bool isCIterSet;

public:

    CustomPolygonRingRange(PolygonRingIterator begin, PolygonRingIterator end) : _begin(begin), _end(end), isIterSet(true) {}
    CustomPolygonRingRange(ConstPolygonRingIterator begin, ConstPolygonRingIterator end) : _cbegin(begin), _cend(end), isCIterSet(true) {}

    PolygonRingIterator begin()
    {
        assert(isIterSet);
        return _begin;
    }

    ConstPolygonRingIterator cbegin() const
    {
        assert(isCIterSet);
        return _cbegin;
    }

    PolygonRingIterator end()
    {
        assert(isIterSet);
        return _end;
    }

    ConstPolygonRingIterator cend() const
    {
        assert(isCIterSet);
        return _cend;
    }
};

namespace boost
{
    // Specialize metafunctions. We must include the range.hpp header.
    // We must open the 'boost' namespace.

    template <>
    struct range_iterator<CustomPolygonRingRange> { typedef PolygonRingIterator type; };

    template<>
    struct range_const_iterator<CustomPolygonRingRange> { typedef ConstPolygonRingIterator type; };

} // namespace 'boost'


// The required Range functions. These should be defined in the same namespace
// as Ring.

inline PolygonRingIterator range_begin(CustomPolygonRingRange& r)
    {return r.begin();}

inline ConstPolygonRingIterator range_begin(const CustomPolygonRingRange& r)
    {return r.cbegin();}

inline PolygonRingIterator range_end(CustomPolygonRingRange& r)
    {return r.end();}

inline ConstPolygonRingIterator range_end(const CustomPolygonRingRange& r)
    {return r.cend();}


namespace boost {
    namespace geometry {
        namespace traits {
            template<> struct tag<procon::polygon::ExpandedPolygon> { typedef polygon_tag type; };
            template<> struct ring_const_type<procon::polygon::ExpandedPolygon> { typedef const bg::model::ring<point_t> & type; };
            template<> struct ring_mutable_type<procon::polygon::ExpandedPolygon> { typedef bg::model::ring<point_t> & type; };
            template<> struct interior_const_type<procon::polygon::ExpandedPolygon> { typedef const CustomPolygonRingRange type; };
            template<> struct interior_mutable_type<procon::polygon::ExpandedPolygon> { typedef CustomPolygonRingRange type; };

            template<> struct exterior_ring<procon::polygon::ExpandedPolygon>
            {
                static bg::model::ring<point_t> & get(procon::polygon::ExpandedPolygon& p)
                {
                    return (p.outer());
                }
                static bg::model::ring<point_t> const& get(procon::polygon::ExpandedPolygon const& p)
                {
                    return (p.outer());
                }
            };

            template<> struct interior_rings<procon::polygon::ExpandedPolygon>
            {
                static CustomPolygonRingRange get(procon::polygon::ExpandedPolygon& p)
                {
                    return CustomPolygonRingRange(PolygonRingIterator(p.interiors.begin()), PolygonRingIterator(p.interiors.end()));
                }
                static const CustomPolygonRingRange get(procon::polygon::ExpandedPolygon const& p)
                {
                    return CustomPolygonRingRange(ConstPolygonRingIterator(p.interiors.begin()), ConstPolygonRingIterator(p.interiors.end()));
                }
            };
        }
    }
}

#endif // EXPANDEDPOLYGON_H
