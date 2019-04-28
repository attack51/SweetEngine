#pragma once


#define SAFE_DELETE(x) if (nullptr != x) { delete x; x = nullptr; }
#define SAFE_DELETE_ARRAY(x) if (nullptr != x) { delete[] x; x = nullptr; }

#define SPTR_SAFE_DELETE(x) if(nullptr != x) { x.reset(); x = nullptr; }
#define UPTR_SAFE_DELETE(x) if(nullptr != x) { x.reset(); x = nullptr; }

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#define CONTAINS_KEY(map, key) map.find(key) != map.end()

#define FORWARD_DECL_PTR(type, name) \
    type name;\
    typedef name* name##Ptr

#define FORWARD_DECL_CONST_PTR(type, name) \
    type name;\
    typedef const name* name##Ptr

#define FORWARD_DECL_SPTR(type, name) \
    type name;\
    typedef std::shared_ptr<name> name##SPtr

#define FORWARD_DECL_CONST_SPTR(type, name) \
    type name;\
    typedef const std::shared_ptr<name> name##SPtr

#define FORWARD_DECL_UPTR(type, name) \
    type name;\
    typedef std::unique_ptr<name> name##UPtr

#define FORWARD_DECL_CONST_UPTR(type, name) \
    type name;\
    typedef const std::unique_ptr<name> name##UPtr

#define FORWARD_DECL_WPTR(type, name) \
    type name;\
    typedef std::weak_ptr<name> name##WPtr

#define FORWARD_DECL_CONST_WPTR(type, name) \
    type name;\
    typedef const std::weak_ptr<name> name##WPtr

#define DECLARE_DELEGATE(name, ret_type, ...) \
    typedef std::function<ret_type(__VA_ARGS__)> name

// unicode text
#define U8_TEXT(str) u8##str
#define WCH_TEXT(str) L##str
#define ANSI_TEXT(str) str

// Min Max
#define Min(lhs, rhs) lhs<rhs?lhs:rhs
#define Max(lhs, rhs) lhs>rhs?lhs:rhs

// EXPORT_TYPE
#define EXPORT_TYPE_MESH    1
#define EXPORT_TYPE_SKEL    2
#define EXPORT_TYPE_ANIM    4

// MESH_EXTRACT_ELM
#define MESH_EXTRACT_ELM_COL  1
#define MESH_EXTRACT_ELM_NOR  2
#define MESH_EXTRACT_ELM_TAN  4
#define MESH_EXTRACT_ELM_UV   8
#define MESH_EXTRACT_ELM_UV2  16
#define MESH_EXTRACT_ELM_SKIN 32

// ETC
#define INDEX_NOT_FOUND -1

// SKIN
#define SKIN_WEIGHT_COUNT 3

// Epsilon
#define POS_EPSILON (1 / 512.0)
#define COL_EPSILON (1 / 256.0)
#define NOR_EPSILON (1 / 512.0)
#define UV_EPSILON  (1 / 256.0)

//Sample
#define DEFAULT_FRAMERATE 30
