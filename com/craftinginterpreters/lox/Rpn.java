package com.craftinginterpreters.lox;

class Rpn implements Expr.Visitor<String> {
    String print(Expr expr) {
        return expr.accept(this);
    }

    // Binary
    @Override
    public String visitBinaryExpr(Expr.Binary expr) {
        return expr.left.accept(this) + " " 
        + expr.right.accept(this) + " "
        + expr.operator.lexeme;
    }

    // Grouping
    @Override
    public String visitGroupingExpr(Expr.Grouping expr) {
        return expr.expression.accept(this);
    }

    // Literal
    @Override
    public String visitLiteralExpr(Expr.Literal expr) {
        return expr.value.toString();
    }

    // Unary
    @Override
    public String visitUnaryExpr(Expr.Unary expr) {
        return expr.right.accept(this) + " "
        + expr.operator.lexeme;
    }

    public static void main(String[] rags) {
        // (1 + 2) * (4 - 3) Example
        Expr expression = new Expr.Binary(
            new Expr.Grouping(
                new Expr.Binary(
                    new Expr.Literal(1),
                    new Token(TokenType.PLUS, "+", null, 1),
                    new Expr.Literal(2))),
            new Token(TokenType.STAR, "*", null, 1),
            new Expr.Grouping(
                new Expr.Binary(
                    new Expr.Literal(4),
                    new Token(TokenType.MINUS, "-", null, 1),
                    new Expr.Literal(3))));

        System.out.println(new Rpn().print(expression));
    }
}