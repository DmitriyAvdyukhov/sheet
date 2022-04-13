#pragma once

#include "cell.h"
#include "common.h"

#include <functional>
#include <unordered_map>

struct HasherPosition
{
    size_t operator()(Position pos) const noexcept
    {
        std::hash<std::string>hstr;
        return static_cast<size_t>(hstr(pos.ToString()));
    }
};

struct EqualPosition
{
    bool operator()(Position lhs, Position rhs) const noexcept
    {
        return lhs.col == rhs.col && lhs.row == rhs.row;
    }
};

class Sheet : public SheetInterface
{
public:
    ~Sheet();

    void SetCell(Position pos, std::string text) override;

    const Cell* GetCell(Position pos) const override;

    Cell* GetCell(Position pos) override;

    void ClearCell(Position pos) override;

    Size GetPrintableSize() const override;

    void PrintValues(std::ostream& output) const override;

    void PrintTexts(std::ostream& output) const override;

	// Можете дополнить ваш класс нужными полями и методами
    std::unique_ptr<Cell> CreateCell(const std::string& str)
    {
        std::unique_ptr<Cell> cell = std::make_unique<Cell>();
        cell->Set(str);
        return cell;
    }
    void SetMinSize(Position pos) noexcept
    {
        if (min_size_.cols <= pos.col)
        {
            min_size_.cols = pos.col + 1;
        }
        if (min_size_.rows <= pos.row)
        {
            min_size_.rows = pos.row + 1;
        }
    }

    void FindMinSize()
    {
        min_size_ = {};
        for (const auto& [pos, data] : sheet_)
        {
            SetMinSize(pos);
        }
    }

private:

    std::unordered_map<Position, std::unique_ptr<Cell>, HasherPosition, EqualPosition> sheet_;
    Size min_size_;
};