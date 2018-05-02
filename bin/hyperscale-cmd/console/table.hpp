/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <iostream>
#include <map>
#include <iomanip>
#include <vector>
#include <string>

namespace hyperscale {
namespace console {

    class Table {
    public:
        enum class Style {
            BORDER,
            LIGHT
        };

        enum class Alignment {
            LEFT,
            RIGHT
        };

        typedef std::vector<std::string> Row;

        Table(char horizontal = '-', char vertical = '|', char corner = '+'):
            m_horizontal(horizontal),
            m_vertical(vertical),
            m_corner(corner) {}

        void setAlignment(std::size_t i, Alignment alignment) {
            m_alignment[i] = alignment;
        }

        Alignment alignment(std::size_t i) const {
            return m_alignment[i];
        }

        void setStyle(Style style) {
            m_style = style;
        }

        Style style() const {
            return m_style;
        }

        char vertical() const {
            return m_vertical;
        }

        char horizontal() const {
            return m_horizontal;
        }

        void add(std::string const & content) {
            m_current.push_back(content);
        }

        void endOfRow() {
            m_rows.push_back(m_current);
            m_current.assign(0, "");
        }

        template <typename Iterator>
        void addRow(Iterator begin, Iterator end) {
            for (auto i = begin; i != end; ++i) {
                add(*i);
            }

            endOfRow();
        }

        template <typename Container>
        void addRow(Container const & container) {
            addRow(container.begin(), container.end());
        }

        std::vector<Row> const & rows() const {
            return m_rows;
        }

        void setup() const {
            determineWidths();
            setupAlignment();
        }

        std::string ruler() const {
            std::string result;

            if (m_style == Style::LIGHT) {
                for (auto width = m_width.begin(); width != m_width.end(); ++width) {
                    result += std::string(*width + 2, m_horizontal);
                }

                return result;
            }


            result += m_corner;

            for (auto width = m_width.begin(); width != m_width.end(); ++width) {
                result += std::string(*width + 2, m_horizontal);
                result += m_corner;
            }

            return result;
        }

        int width(std::size_t i) const {
            return m_width[i];
        }

    private:
        char m_horizontal;
        char m_vertical;
        char m_corner;
        Row m_current;
        std::vector<Row> m_rows;
        std::vector<std::size_t> mutable m_width;
        std::map<std::size_t, Alignment> mutable m_alignment;
        Style mutable m_style;

        std::size_t columns() const {
            return m_rows[0].size();
        }

        void determineWidths() const {
            m_width.assign(columns(), 0);

            for (auto rowIterator = m_rows.begin(); rowIterator != m_rows.end(); ++rowIterator) {
                Row const & row = * rowIterator;
                for (std::size_t i = 0; i < row.size(); ++i) {
                    m_width[i] = m_width[i] > row[i].size() ? m_width[i] : row[i].size();
                }
            }
        }

        void setupAlignment() const {
            for (std::size_t i = 0; i < columns(); ++i) {
                if (m_alignment.find(i) == m_alignment.end() ) {
                    m_alignment[i] = Alignment::LEFT;
                }
            }
        }
    };

    std::ostream & operator<<(std::ostream& stream, Table const& table) {
        table.setup();

        if (table.style() == Table::Style::BORDER) {
            stream << table.ruler() << std::endl;
        }

        for (auto rowIterator = table.rows().begin(); rowIterator != table.rows().end(); ++rowIterator) {
            Table::Row const& row = *rowIterator;

            if (table.style() == Table::Style::BORDER) {
                stream << table.vertical();
            }

            for (std::size_t i = 0; i < row.size(); ++i) {
                auto alignment = table.alignment(i) == Table::Alignment::LEFT ? std::left : std::right;
                stream << " ";
                stream << std::setw(table.width(i)) << alignment << row[i];
                stream << " ";
                if (table.style() == Table::Style::BORDER) {
                    stream << table.vertical();
                }
            }

            stream << std::endl;

            if (table.style() == Table::Style::BORDER || std::distance(table.rows().begin(), rowIterator) == 0) {
                stream << table.ruler() << std::endl;
            }
        }

        return stream;
    }

} // end of console namespace
} // end of hyperscale namespace


