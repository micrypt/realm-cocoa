/*************************************************************************
 *
 * TIGHTDB CONFIDENTIAL
 * __________________
 *
 *  [2011] - [2012] TightDB Inc
 *  All Rights Reserved.
 *
 * NOTICE:  All information contained herein is, and remains
 * the property of TightDB Incorporated and its suppliers,
 * if any.  The intellectual and technical concepts contained
 * herein are proprietary to TightDB Incorporated
 * and its suppliers and may be covered by U.S. and Foreign Patents,
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from TightDB Incorporated.
 *
 **************************************************************************/
#ifndef TIGHTDB_COLUMN_BINARY_HPP
#define TIGHTDB_COLUMN_BINARY_HPP

#include <tightdb/column.hpp>
#include <tightdb/array_binary.hpp>

namespace tightdb {


class ColumnBinary : public ColumnBase {
public:
    ColumnBinary(Allocator& = Allocator::get_default());
    ColumnBinary(size_t ref, ArrayParent* = 0, size_t ndx_in_parent = 0,
                 Allocator& = Allocator::get_default());
    ~ColumnBinary();

    void Destroy();

    size_t Size() const TIGHTDB_NOEXCEPT TIGHTDB_OVERRIDE;
    bool is_empty() const TIGHTDB_NOEXCEPT;

    BinaryData get(std::size_t ndx) const TIGHTDB_NOEXCEPT;

    void add() TIGHTDB_OVERRIDE { add(BinaryData()); }
    void add(BinaryData value);
    void set(std::size_t ndx, BinaryData value);
    void insert(std::size_t ndx) TIGHTDB_OVERRIDE { insert(ndx, BinaryData()); }
    void insert(std::size_t ndx, BinaryData value);
    void erase(std::size_t ndx) TIGHTDB_OVERRIDE;
    void Resize(std::size_t ndx);
    void Clear() TIGHTDB_OVERRIDE;
    void fill(std::size_t count);

    // Requires that the specified entry was inserted as StringData.
    StringData get_string(std::size_t ndx) const TIGHTDB_NOEXCEPT;

    void add_string(StringData value);
    void set_string(std::size_t ndx, StringData value);
    void insert_string(std::size_t ndx, StringData value);

    // Index
    bool HasIndex() const {return false;}
    void BuildIndex(Index&) {}
    void ClearIndex() {}
    size_t FindWithIndex(int64_t) const {return (size_t)-1;}

    size_t GetRef() const {return m_array->GetRef();}
    void SetParent(ArrayParent *parent, size_t pndx) {m_array->SetParent(parent, pndx);}
    void UpdateParentNdx(int diff) {m_array->UpdateParentNdx(diff);}

    /// Compare two binary columns for equality.
    bool compare(const ColumnBinary&) const;

#ifdef TIGHTDB_DEBUG
    void Verify() const {}; // Must be upper case to avoid conflict with macro in ObjC
#endif // TIGHTDB_DEBUG

protected:
    friend class ColumnBase;

    void UpdateRef(size_t ref);

    BinaryData LeafGet(size_t ndx) const TIGHTDB_NOEXCEPT;
    void LeafSet(size_t ndx, BinaryData value);
    void LeafInsert(size_t ndx, BinaryData value);
    void LeafDelete(size_t ndx);

#ifdef TIGHTDB_DEBUG
    virtual void LeafToDot(std::ostream& out, const Array& array) const;
#endif // TIGHTDB_DEBUG

private:
    void add(StringData value) { add_string(value); }
    void set(std::size_t ndx, StringData value) { set_string(ndx, value); }
    void LeafSet(size_t ndx, StringData value);
    void LeafInsert(size_t ndx, StringData value);
};




// Implementation

inline BinaryData ColumnBinary::get(std::size_t ndx) const TIGHTDB_NOEXCEPT
{
    TIGHTDB_ASSERT(ndx < Size());
    return ArrayBinary::column_get(m_array, ndx);
}

inline StringData ColumnBinary::get_string(std::size_t ndx) const TIGHTDB_NOEXCEPT
{
    BinaryData bin = get(ndx);
    TIGHTDB_ASSERT(0 < bin.size());
    return StringData(bin.data(), bin.size()-1);
}

inline void ColumnBinary::add(BinaryData value)
{
    insert(Size(), value);
}

inline void ColumnBinary::add_string(StringData value)
{
    insert_string(Size(), value);
}

} // namespace tightdb

#endif // TIGHTDB_COLUMN_BINARY_HPP