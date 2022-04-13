#include "sheet.h"

#include "cell.h"
#include "common.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>

using namespace std::literals;

Sheet::~Sheet() {}

void Sheet::SetCell(Position pos, std::string text)
{
    if (!pos.IsValid())
    {
        throw InvalidPositionException("Invalid position");
    }
    if (sheet_.find(pos) == sheet_.end())
    {
        auto cell = CreateCell(text);
        SetMinSize(pos);
        sheet_.emplace(std::move(pos) , std::move(cell));
    }
    else
    {
        SetMinSize(pos);
        sheet_.at(pos)->Set(std::move(text));
    }
}

const Cell* Sheet::GetCell(Position pos) const
{    
    if (!pos.IsValid())
    {
        throw InvalidPositionException("Invalid position");
    }
    if (sheet_.find(pos) != sheet_.end())
    {
        return sheet_.at(pos).get();
    }
    else
    {
        return nullptr;
    }
}

Cell* Sheet::GetCell(Position pos) 
{
    if (!pos.IsValid())
    {
        throw InvalidPositionException("Invalid position");
    }
    if (sheet_.find(pos) != sheet_.end())
    {
        return sheet_.at(pos).get();
    }
    else
    {
        return nullptr;
    }
}

void Sheet::ClearCell(Position pos)
{
    if (!pos.IsValid()) 
    {
        throw InvalidPositionException("Invalid position");
    }
    auto it = sheet_.find(pos);
    if (it != sheet_.end())
    {
        it->second.reset(nullptr);
        sheet_.erase(it);
    }
    FindMinSize();
}

Size Sheet::GetPrintableSize() const
{    
    return min_size_;
}

void Sheet::PrintValues(std::ostream& output) const
{
    Size size = GetPrintableSize();
    for (int r = 0; r < size.rows; ++r) 
    {
        for (int c = 0; c < size.cols; ++c) 
        {
            if (c > 0)
            {
                output << "\t";
            }
            const auto& it = sheet_.find({ r, c });
            if (it != sheet_.end() && it->second != nullptr && !it->second->GetText().empty())
            {
                std::visit([&](const auto& value) 
                    {
                        output << value; 
                    }
                    , it->second->GetValue());
            }
        }
        output << "\n";
    }
}

void Sheet::PrintTexts(std::ostream& output) const
{
    Size size = GetPrintableSize();
    for (int r = 0; r < size.rows; ++r) 
    {
        for (int c = 0; c < size.cols; ++c) 
        {
            if (c > 0)
            {
                output << "\t";
            }
            const auto& it = sheet_.find({ r, c });
            if (it != sheet_.end() && it->second != nullptr && !it->second->GetText().empty())
            {
                output << it->second->GetText();
            }
        }
        output << "\n";
    }
}

std::unique_ptr<SheetInterface> CreateSheet() 
{
    return std::make_unique<Sheet>();
}