
//Basic often used Boost includes........

#include <boost/system/error_code.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/shared_array.hpp>

#include <boost/filesystem.hpp>
namespace bfs= boost::filesystem;

// .........................................
namespace boost{ namespace filesystem {
typedef std::vector< bfs::path > vpath;
typedef std::list< bfs::path > list_path;

namespace path_traits {
  inline
    void convert(const CString& from,
    std::string & to,
    const codecvt_type&)
  {
    //BOOST_ASSERT(from);
    //to += (LPCTSTR)from;
  }
}}};

//..............................................................
//for: inline void HeSplitCString( LPCTSTR spInlist, VCString& vOutlist )
//#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

#include <boost/date_time/gregorian/gregorian.hpp> //include all types plus i/o
namespace bgdt= boost::gregorian;
