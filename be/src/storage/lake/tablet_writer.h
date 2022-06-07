// This file is licensed under the Elastic License 2.0. Copyright 2021-present, StarRocks Limited.

#pragma once

#include <string>
#include <vector>

#include "common/status.h"

namespace starrocks::vectorized {
class Chunk;
} // namespace starrocks::vectorized

namespace starrocks::lake {

class Tablet;

// Basic interface for tablet writers.
class TabletWriter {
public:
    virtual ~TabletWriter() = default;

    virtual int64_t tablet_id() const = 0;

    // This method is called immediately before any elements are processed, it
    // should contain the writer's initialization logic.
    virtual Status open() = 0;

    // Writes elements from the specified chunk to this rowset.
    //
    // It's guaranteed that the elements in each chunk are arranged in ascending
    // order, and the elements among all chunks written before `flush()` are also
    // arranged in ascending order.
    virtual Status write(const starrocks::vectorized::Chunk& data) = 0;

    // Flushes this writer and forces any buffered bytes to be written out to segment files.
    // There is no order guarantee between the data written before a `flush()`
    // and the data written after it.
    virtual Status flush() = 0;

    // This method is called at the end of data processing.
    virtual Status finish() = 0;

    // This method is called at the very end of the operator's life, both in
    // the case of a successful completion of the operation, and in the case
    // of a failure and canceling.
    virtual void close() = 0;

    // Return a list of files generated by this writer.
    // The file paths are relative to the tablet group path.
    //
    // PREREQUISITES: the writer has successfully `finish()`ed but not yet `close()`ed.
    virtual std::vector<std::string> files() const = 0;

    // The sum of all segment file sizes, in bytes.
    virtual int64_t data_size() const = 0;

    // The total number of rows have been written.
    virtual int64_t num_rows() const = 0;
};

} // namespace starrocks::lake
