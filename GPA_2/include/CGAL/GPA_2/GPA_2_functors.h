// TODO: Add licence
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL:$
// $Id: $
// 
//
// Author(s)     : Eric Berberich <eric@mpi-inf.mpg.de>
//                 Pavel Emeliyanenko <asm@mpi-sb.mpg.de>
//
// ============================================================================

/*! \file GPA_2/GPA_2_functors.h
 *  \brief defines GPA_2 function objects 
 */

#ifndef CGAL_GPA_2_functors_H
#define CGAL_GPA_2_functors_H

CGAL_BEGIN_NAMESPACE

namespace GPA_2_Functors {

template <class GPA_2>
class Compare_x_2
{
    typedef typename GPA_2::Xy_coordinate_2 Point_2;
    typedef typename GPA_2::Arc_2 Arc_2;

    typedef CGAL::Comparison_result result_type;
    typedef Arity_tag<4>            Arity;

    /*!
     * Compare the x-coordinates of two points.
     * \param p1 The first point.
     * \param p2 The second point.
     * \return LARGER if x(p1) > x(p2);
     *         SMALLER if x(p1) \< x(p2);
     *         EQUAL if x(p1) = x(p2).
     */
    result_type operator()(const Point_2 &p1, const Point_2 &p2) const
    { 
        typename GPA_2::Curve_kernel_2 kernel_2;
        return kernel_2.compare_x_2_object()(p1, p2);
    }

    /*!
     * Compare the relative positions of a vertical curve and an unbounded end
     * of the curve \c cv
     * \param p A reference point; we refer to a vertical line incident to p.
     * \param cv The compared curve.
     * \param end MIN_END if we refer to cv's minimal end,
     *            MAX_END if we refer to its maximal end.
     * \pre cv's relevant end is defined at y = +/- oo.
     * \return SMALLER if p lies to the left of cv;
     *         LARGER if p lies to the right cv;
     *         EQUAL in case of an overlap.
     */
    result_type operator()(const Point_2& p, const Arc_2& cv, 
            ::CGAL::Curve_end end) const 
    {
        return (cv.compare_end(end, p));
    }

   /*!
     * Compare the relative positions of the unbounded curve ends of \c cv1
     * and \c cv2
     * \param cv1 The first curve.
     * \param end1 MIN_END if we refer to cv1's minimal end,
     *             MAX_END if we refer to its maximal end.
     * \param cv2 The second curve.
     * \param end2 MIN_END if we refer to cv2's minimal end,
     *             MAX_END if we refer to its maximal end.
     * \pre the curve ends have a bounded x-coord and unbounded y-coord,
          namely each of \c cv1 and \c cv2 is vertical or asymptotic
     * \return SMALLER if cv1 lies to the left of cv2;
     *         LARGER if cv1 lies to the right cv2;
     *         EQUAL in case of an overlap.
     */
    result_type operator()(const Arc_2& cv1, ::CGAL::Curve_end end1, 
             const Arc_2& cv2, ::CGAL::Curve_end end2) const {
        return cv1.compare_ends(end1, cv2, end2);
    }
};

template < class GPA_2 >
class Compare_xy_2
{
    typedef typename GPA_2::Xy_coordinate_2 Point_2;
    typedef typename GPA_2::Arc_2 Arc_2;
   
public:
    typedef CGAL::Comparison_result result_type;
    typedef Arity_tag<2>            Arity;

    /*!
     * Compares two points lexigoraphically: by x, then by y.
     * \param p1 The first point.
     * \param p2 The second point.
     * \return LARGER if x(p1) > x(p2), or if x(p1) = x(p2) and y(p1) > y(p2);
     *         SMALLER if x(p1) \< x(p2), or if x(p1) = x(p2) and 
     *                   y(p1) \< y(p2);
     *         EQUAL if the two points are equal.
     */
    result_type operator()(const Point_2& p1, const Point_2& p2) const
    {
        typename GPA_2::Curve_kernel_2 kernel_2;
        return (kernel_2.compare_xy_2_object()(p1, p2));
    }
};

//!\brief Tests two objects, whether they are equal
template < class GPA_2 >
class Equal_2
{
    typedef typename GPA_2::Xy_coordinate_2 Point_2;
    typedef typename GPA_2::Arc_2 Arc_2;
   
public:
    typedef bool result_type;
    typedef Arity_tag<2> Arity;
    
    /*!
     * Check if the two points are the same.
     * \param p1 The first point.
     * \param p2 The second point.
     * \return (true) if the two point are the same; (false) otherwise.
     */
    result_type operator()(const Point_2& p1, const Point_2& p2) const {
    
        if(&p1 == &p2) 
            return true;
        typename GPA_2::Curve_kernel_2 kernel_2;
        return (kernel_2.compare_xy_2_object()(p1, p2) == CGAL::EQUAL);
    }
     
    /*!
     * Check if the two x-monotone curves are the same (have the same graph).
     * \param cv1 The first curve.
     * \param cv2 The second curve.
     * \return (true) if the two curves are the same; (false) otherwise.
     */
    result_type operator()(const Arc_2& cv1, const Arc_2& cv2) const {
    
        if(&cv1 == &cv2)
            return true; 
        return cv1.is_equal(cv2);
    }
};

template < class GPA_2 >
class Is_vertical_2 {
{
    typedef typename GPA_2::Arc_2 Arc_2;
   
public:
    typedef bool result_type;
    typedef Arity_tag<1> Arity;

    /*!
     * Check whether the given x-monotone curve is a vertical segment.
     * \param cv The curve.
     * \return (true) if the curve is a vertical segment; 
     * (false) otherwise.
     */
    result_type operator()(const Arc_2& cv) const {
        return cv.is_vertical();
    }
};

template < class GPA_2 >
class Construct_min_vertex_2 
{
    typedef typename GPA_2::Xy_coordinate_2 Point_2;
    typedef typename GPA_2::Arc_2 Arc_2;
   
public:
    typedef Point_2 result_type;
    typedef Arity_tag<1> Arity;

    /*!
     * Get the left end-point of the x-monotone curve
     * \param cv The curve.
     * \pre corresponding end-point must not lie at infinity
     * \return The left endpoint.
     */
    result_type operator()(const Arc_2& cv) const {
    
        CGAL_precondition(
            cv.get_boundary_in_x(CGAL::MIN_END) == CGAL::NO_BOUNDARY &&
            cv.get_boundary_in_y(CGAL::MIN_END) == CGAL::NO_BOUNDARY);
        return cv.get_curve_end(CGAL::MIN_END);
    }
};

template < class GPA_2 >
class Construct_max_vertex_2 
{
    typedef typename GPA_2::Xy_coordinate_2 Point_2;
    typedef typename GPA_2::Arc_2 Arc_2;
   
public:
    typedef Point_2 result_type;
    typedef Arity_tag<1> Arity;

    /*!
     * Get the right endpoint of the x-monotone curve (segment).
     * \param cv The curve.
     * \pre corresponding end-point must not lie at infinity
     * \return The right endpoint.
     */
    result_type operator()(const Arc_2& cv) const {
    
        CGAL_precondition(
            cv.get_boundary_in_x(CGAL::MAX_END) == CGAL::NO_BOUNDARY &&
            cv.get_boundary_in_y(CGAL::MAX_END) == CGAL::NO_BOUNDARY);
        return cv.get_curve_end(CGAL::MAX_END);
    }
};


// TODO existing for backward compatability - will be removed in future
// do we need this ?
template < class GPA_2 >
class Infinite_in_x_2 
{
    typedef typename GPA_2::Xy_coordinate_2 Point_2;
    typedef typename GPA_2::Arc_2 Arc_2;
   
public:
    typedef ::CGAL::Boundary_type result_type;
    typedef Arity_tag<2> Arity;

    result_type operator()(const Arc_2 & cv, 
            ::CGAL::Curve_end end) const {
        CGAL_error("The functor is considered deprecated ?!");
        return CGAL::NO_BOUNDARY;
    }
};

// TODO existing for backward compatability - will be removed in future
// do we need this ?
template < class GPA_2 >
class Infinite_in_y_2 
{
    typedef typename GPA_2::Arc_2 Arc_2;
   
public:
    typedef ::CGAL::Boundary_type result_type;
    typedef Arity_tag<2> Arity;

    result_type operator()(const Arc_2 & cv, 
            ::CGAL::Curve_end end) const {
        CGAL_error("The functor is considered deprecated ?!");
        return CGAL::NO_BOUNDARY;
    }
};

template < class GPA_2 >
class Boundary_in_x_2 
{
    typedef typename GPA_2::Arc_2 Arc_2;
   
public:
    typedef ::CGAL::Boundary_type result_type;
    typedef Arity_tag<2> Arity;

    /*!
     * Check if an end of a given x-monotone curve is infinite at x.
     * \param cv The curve.
     * \param ind MIN_END if we refer to cv's minimal end,
     *            MIN_END if we refer to its maximal end.
     * \return MINUS_INFINITY if the curve end lies at x = -oo;
     *         NO_BOUNDARY if the curve end has a finite x-coordinate;
     *         PLUS_INFINITY if the curve end lies at x = +oo.
     */
    result_type operator()(const Arc_2 & cv, 
            ::CGAL::Curve_end end) const {
            
        return cv.get_boundary_in_x(end);
    }
};

template < class GPA_2 >
class Boundary_in_y_2 
{
    typedef typename GPA_2::Arc_2 Arc_2;
   
public:
    typedef ::CGAL::Boundary_type result_type;
    typedef Arity_tag<2> Arity;

    /*!
     * Check if an end of a given x-monotone curve is infinite at x.
     * \param cv The curve.
     * \param ind MIN_END if we refer to cv's minimal end,
     *            MIN_END if we refer to its maximal end.
     * \return MINUS_INFINITY if the curve end lies at x = -oo;
     *         NO_BOUNDARY if the curve end has a finite x-coordinate;
     *         PLUS_INFINITY if the curve end lies at x = +oo.
     */
    result_type operator()(const Arc_2 & cv, 
            ::CGAL::Curve_end end) const {
            
        return cv.get_boundary_in_y(end);
    }
};

template < class GPA_2 >
class Compare_y_at_x_2
{
    typedef typename GPA_2::Xy_coordinate_2 Point_2;
    typedef typename GPA_2::Arc_2 Arc_2;
   
public:
    typedef CGAL::Comparison_result result_type;
    typedef Arity_tag<3>            Arity;

public:
    /*!
     * Return the location of the given point with respect to the input curve.
     * \param cv The curve.
     * \param p The point.
     * \pre p is in the x-range of cv.
     * \return SMALLER if y(p) \< cv(x(p)), i.e. the point is below the curve;
     *         LARGER if y(p) > cv(x(p)), i.e. the point is above the curve;
     *         EQUAL if p lies on the curve.
     */
    result_type operator()(const Point_2& p, const Arc_2& cv) const
    {
      return (cv.compare_y_at_x(p));
    }

    /*!
     * Compare the relative y-positions of two curves at x = +/- oo.
     * \param cv1 The first curve.
     * \param cv2 The second curve.
     * \param end MIN_END if we compare at x = -oo;
     *            MAX_END if we compare at x = +oo.
     * \pre The curves are defined at x = +/- oo.
     * \return SMALLER if cv1 lies below cv2;
     *         LARGER if cv1 lies above cv2;
     *         EQUAL in case of an overlap.
     */
    result_type operator()(const Arc_2& cv1, const Arc_2& cv2, 
        CGAL::Curve_end end) const
    {
        return (cv1.compare_y_at_x(cv2, end));
    }
};

template < class GPA_2 >
class Compare_y_at_x_left_2
{
    typedef typename GPA_2::Xy_coordinate_2 Point_2;
    typedef typename GPA_2::Arc_2 Arc_2;
   
public:
    typedef CGAL::Comparison_result result_type;
    typedef Arity_tag<3>            Arity;

    /*!
     * Compares the y value of two x-monotone curves immediately to the left
     * of their intersection point. If one of the curves is vertical
     * (emanating downward from p), it's always considered to be below the
     * other curve.
     * \param cv1 The first curve.
     * \param cv2 The second curve.
     * \param p The intersection point.
     * \pre The point p lies on both curves, and both of them must be also be
     *      defined (lexicographically) to its left.
     * \return The relative position of cv1 with respect to cv2 immdiately to
     *         the left of p: SMALLER, LARGER or EQUAL.
     */
    result_type operator() (const Arc_2& cv1, const Arc_2& cv2,
             const Point_2& p) const 
    {
        return (cv1.compare_y_at_x_left(cv2, p));
    }
};

template < class GPA_2 >
class Compare_y_at_x_right_2
{
    typedef typename GPA_2::Xy_coordinate_2 Point_2;
    typedef typename GPA_2::Arc_2 Arc_2;
   
public:
    typedef CGAL::Comparison_result result_type;
    typedef Arity_tag<3>            Arity;

     /*!
     * Compares the y value of two x-monotone curves immediately 
     * to the right of their intersection point. If one of the curves is
     * vertical (emanating upward from p), it's always considered to be above
     * the other curve.
     * \param cv1 The first curve.
     * \param cv2 The second curve.
     * \param p The intersection point.
     * \pre The point p lies on both curves, and both of them must be 
     * also be defined (lexicographically) to its right.
     * \return The relative position of cv1 with respect to 
     * cv2 immdiately to the right of p: SMALLER, LARGER or EQUAL.
     */
    result_type operator() (const Arc_2& cv1, const Arc_2& cv2,
             const Point_2& p) const 
    {
        return (cv1.compare_y_at_x_right(cv2, p));
    }
};


template < class GPA_2 >
class Are_mergeable_2 
{
    typedef typename GPA_2::Xy_coordinate_2 Point_2;
    typedef typename GPA_2::Arc_2 Arc_2;
   
public:
    typedef bool result_type;
    typedef Arity_tag<2> Arity;    
    
    /*!\brief
     * Check whether two given curves (arcs) are mergeable
     * \param cv1 The first curve.
     * \param cv2 The second curve.
     * \return (true) if the two arcs are mergeable, i.e., they are supported
     * by the same curve and share a common endpoint; (false) otherwise.
     */
    bool operator()(const Arc_2& cv1, const Arc_2 & cv2) const {
    
        typename GAPS::Are_mergeable_2 are_mergeable(
                gaps_.are_mergeable_2_object()
        );
        return are_mergeable(cv1, cv2);
    }
};

template < class GPA_2 >
class Merge_2
{
    typedef typename GPA_2::Xy_coordinate_2 Point_2;
    typedef typename GPA_2::Arc_2 Arc_2;
   
public:
    typedef void result_type;
    typedef Arity_tag<2> Arity;    

    /*!\brief
     * Merge two given x-monotone curves into a single one
     * \param cv1 The first curve.
     * \param cv2 The second curve.
     * \param c Output: The resulting curve.
     * \pre The two curves are mergeable, that is they are supported by the
     *      same curve and share a common endpoint.
     */  
    void operator()(const Arc_2& cv1, const Arc_2 & cv2, Arc_2& c) const {
    
        c = cv1.merge(cv2);
    }
};

template < class GPA_2 >
class Split_2 
{
    typedef typename GPA_2::Xy_coordinate_2 Point_2;
    typedef typename GPA_2::Arc_2 Arc_2;
   
public:
    typedef void result_type;
    typedef Arity_tag<4> Arity;    
    
    /*!
     * Split a given x-monotone curve at a given point into two sub-curves.
     * \param cv The curve to split
     * \param p The split point.
     * \param c1 Output: The left resulting subcurve (p is its right endpoint)
     * \param c2 Output: The right resulting subcurve (p is its left endpoint)
     * \pre p lies on cv but is not one of its end-points.
     */
    void operator()(const Arc_2& cv, const Point_2 & p,
            Arc_2& c1, Arc_2& c2) const {

        cv.split(p, c1, c2);            
    }
};

template < class GPA_2 >
class Intersect_2  
{
    typedef typename GPA_2::Xy_coordinate_2 Point_2;
    typedef typename GPA_2::Arc_2 Arc_2;
   
public:
    typedef Arity_tag<3> Arity;    
    
    /*!
     * Find the intersections of the two given curves and insert 
     * them to the given output iterator. As two segments may 
     * intersect only once, only a single will be contained in the iterator.
     * \param cv1 The first curve.
     * \param cv2 The second curve.
     * \param oi The output iterator.
     * \return The past-the-end iterator.
     */
    template <class OutputIterator>
    OutputIterator operator()(const Arc_2& cv1, const Arc_2& cv2,
                               OutputIterator oi) {
        
        if(GPA_2().do_overlap_2_object()(cv1, cv2)) {
            typedef typename GAPS::Point_2 Point_2;
            typedef typename GAPS::Segment_2 Segment_2;
            
            typename GAPS::Less_xy_2 less_xy(
                    gaps_.less_xy_2_object()
            );
            typename GAPS::Is_vertical_2 is_vertical(
                    gaps_.is_vertical_2_object()
            );
            typename GAPS::Trim_2 trim(
                    gaps_.trim_2_object()
            );
            
            typedef std::pair< Point_2, Point_2 >  Endpoint_pair;
            
            // TODO use construct_min/max_vertex 
            
            // ordered endpoints have source is left (or bottom) 
            // endpoint and target
            // as the right (or top) endpoint
            Endpoint_pair c1_endpoints;
            c1_endpoints.first = source(cv1);
            c1_endpoints.second = target(cv1);
            if (!less_xy(c1_endpoints.first, c1_endpoints.second)) {
                std::swap(c1_endpoints.first, c1_endpoints.second);
            }
            
            Endpoint_pair c2_endpoints;
            c2_endpoints.first = source(cv2);
            c2_endpoints.second = target(cv2);
            if (!less_xy(c2_endpoints.first, c2_endpoints.second)) {
                std::swap(c2_endpoints.first, c2_endpoints.second);
            }
            
            Point_2 greater_source, smaller_target;
            
            // determine greater source7
            greater_source = std::max(c1_endpoints.first, 
                                      c2_endpoints.first,
                                      less_xy);
            
            // determine smaller target
            smaller_target = std::min(c1_endpoints.second, 
                                      c2_endpoints.second,
                                      less_xy);
            Segment_2 s = trim(cv2, greater_source, smaller_target);
            *oi++ = CGAL::make_object(s);
            
        } else {
            typename GAPS::Intersect_2 intersect(
                    gaps_.intersect_2_object()
            );
            typedef std::vector< typename GAPS::Point_2 > Container;
            typedef typename Container::const_iterator Const_iterator;
            typedef std::pair< typename GAPS::Point_2, unsigned int > 
                Point_and_mult;
            Container tmp;
            intersect(cv1, cv2, std::back_inserter(tmp));
            for (Const_iterator it = tmp.begin(); it != tmp.end(); it++) {
        typename GAPS::Point_is_equal_2 point_is_equal(
                        gaps_.point_is_equal_2_object()
                ); 
        if (point_is_equal(source(cv1),*it) ||
                    point_is_equal(target(cv1),*it) ||
                    point_is_equal(source(cv2),*it) ||
                    point_is_equal(target(cv2),*it)) {
                    *oi++ = CGAL::make_object(Point_and_mult(*it, 0));
                } else {
                    typename GAPS::Multiplicity_of_intersection_2 
                        multiplicity_of_intersection(
                                gaps_.multiplicity_of_intersection_2_object()
                        );
                    Point_and_mult pam(
                            *it, multiplicity_of_intersection(cv1,cv2,*it)
                    );
                    *oi++ = CGAL::make_object(pam);
                }
            }
        }
        return oi;
    }
};

template < class GPA_2>
class Make_x_monotone_2 
{
    typedef typename GPA_2::Xy_coordinate_2 Point_2;
    typedef typename GPA_2::Arc_2 Arc_2;
   
public:
    typedef void result_type;
    typedef Arity_tag<2> Arity;    

    /*!
     * decompose a given curve (or arc) into list of x-monotone pieces 
     * (subcurves) and insert them to the output iterator
     * \param cv The curve.
     * \param oi The output iterator, whose value-type is Object. 
     * The returned objects are all wrappers X_monotone_curve_2 objects.
     * \return The past-the-end iterator.
     */
    template<class OutputIterator>
    OutputIterator operator() (
            const typename InputTraits_2::Input_object_2& cv, 
            OutputIterator oi) {
            
        typename InputTraits_2::Make_sweepable_2 make_sweepable(
                InputTraits_2().make_sweepable_2_object()
        );
        typedef std::vector< typename GAPS::Segment_2 > Container;
        typedef typename Container::const_iterator Const_iterator;
        Container tmp;
        make_sweepable(cv, std::back_inserter(tmp));
        for (Const_iterator it = tmp.begin(); it != tmp.end(); it++) {
            typename GAPS::Is_degenerate_2 is_degenerate(
                    gaps_.is_degenerate_2_object()
            );
            if (is_degenerate(*it)) {
                typename GAPS::Source_2 source(
                        gaps_.source_2_object()
                );
                // CGAL expects points instead of degenerate segments
                *oi++ = CGAL::make_object(source(*it));
            } else {
                typename GAPS::Is_reversed_2 is_reversed(
                        gaps_.is_reversed_2_object()
                );
                // Remark (Ron's mail):
                // As we currently define it, an x-monotone curve does 
                // not need to have a source and a target, because this 
                // implies that it is directed. We view x-monotone curves 
                // as continuous graphs of fucntions, which are undirected. 
                // However, a curve has a "left" (minimal) vertex and a 
                // "right" (maximal) vertex.
                //
                if (!is_reversed(*it)) {
                    *oi++ = CGAL::make_object(*it);  
                } else {
                    // CGAL directed versus left-right
                    typename GAPS::New_endpoints_opposite_2 
                        new_endpoints_opposite(
                                gaps_.new_endpoints_opposite_2_object()
                        );
                    typename GAPS::Source_2 source(
                            gaps_.source_2_object()
                    );
                    typename GAPS::Target_2 target(
                            gaps_.target_2_object()
                    );
                    typename GAPS::Segment_2 s = 
                        new_endpoints_opposite(*it, source(*it), target(*it));
                    *oi++ = CGAL::make_object(s);
                }
            }
        }
        // -> output contains points (instead of degenerated segments) and
        //    from left to right oriented non-degenerated segments
        return oi;
        
    }
};

} // GPA_2_Functors

CGAL_END_NAMESPACE

#endif // CGAL_GPA_2_functors_H
