#ifndef EXPORTERS_H
#define EXPORTERS_H

#include <string>
#include "nlohmann/json.hpp"

class Canvas;
class HistoryManager;

class Exporter {
public:
    virtual ~Exporter() = default;
    virtual bool exportTo(Canvas* canvas, const std::string& path) = 0;
    virtual std::string getFormatName() const = 0;
};

class TXTExporter : public Exporter {
public:
    bool exportTo(Canvas* canvas, const std::string& path) override;
    std::string getFormatName() const override { return "TXT"; }
};

class ANSIExporter : public Exporter {
public:
    bool exportTo(Canvas* canvas, const std::string& path) override;
    std::string getFormatName() const override { return "ANSI"; }
};

class CSVExporter : public Exporter {
public:
    bool exportTo(Canvas* canvas, const std::string& path) override;
    std::string getFormatName() const override { return "CSV"; }
};

class JSONExporter : public Exporter {
public:
    bool exportTo(Canvas* canvas, const std::string& path) override;
    std::string getFormatName() const override { return "JSON"; }
};

class ASCPExporter : public Exporter {
public:
    bool exportTo(Canvas* canvas, const std::string& path) override;
    std::string getFormatName() const override { return "ASCP"; }
};

class ASCPImporter {
public:
    static Canvas* importFrom(const std::string& path, HistoryManager* history);
    static bool validate(const std::string& path);
};

#endif