/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <string>
#include <memory>

namespace hyperscale {
namespace ast {

    enum BinOpType {
        BinOpTypeInvalid,
        BinOpTypeAssign,
        BinOpTypeAssignTimes,
        BinOpTypeAssignTimesWrap,
        BinOpTypeAssignDiv,
        BinOpTypeAssignMod,
        BinOpTypeAssignPlus,
        BinOpTypeAssignPlusWrap,
        BinOpTypeAssignMinus,
        BinOpTypeAssignMinusWrap,
        BinOpTypeAssignBitShiftLeft,
        BinOpTypeAssignBitShiftRight,
        BinOpTypeAssignBitAnd,
        BinOpTypeAssignBitXor,
        BinOpTypeAssignBitOr,
        BinOpTypeAssignMergeErrorSets,
        BinOpTypeBoolOr,
        BinOpTypeBoolAnd,
        BinOpTypeCmpEq,
        BinOpTypeCmpNotEq,
        BinOpTypeCmpLessThan,
        BinOpTypeCmpGreaterThan,
        BinOpTypeCmpLessOrEq,
        BinOpTypeCmpGreaterOrEq,
        BinOpTypeBinOr,
        BinOpTypeBinXor,
        BinOpTypeBinAnd,
        BinOpTypeBitShiftLeft,
        BinOpTypeBitShiftRight,
        BinOpTypeAdd,
        BinOpTypeAddWrap,
        BinOpTypeSub,
        BinOpTypeSubWrap,
        BinOpTypeMult,
        BinOpTypeMultWrap,
        BinOpTypeDiv,
        BinOpTypeMod,
        BinOpTypeUnwrapMaybe,
        BinOpTypeArrayCat,
        BinOpTypeArrayMult,
        BinOpTypeErrorUnion,
        BinOpTypeMergeErrorSets,
    };

    class Node;

    class NodeBinaryOperatorExpression {
    private:
        Node* m_left;

        std::string m_operator;

        Node* m_right;

    public:
        NodeBinaryOperatorExpression() {}
        ~NodeBinaryOperatorExpression() {}

        void setLeft(Node* left) {
            m_left = left;
        }

        Node* getLeft() const {
            return m_left;
        }

        void setOperator(std::string op) {
            m_operator = op;
        }

        std::string getOperator() const {
            return m_operator;
        }


        void setRight(Node* left) {
            m_right = right;
        }

        Node* getRight() const {
            return m_right;
        }
    };

} // end of ast namespace
} // end of hyperscale namespace
