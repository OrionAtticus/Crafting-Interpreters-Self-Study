package com.craftinginterpreters.lox;

import java.util.List;

class LoxFunction implements LoxCallable {
    private final Stmt.Function declaration;
    private final Environment closure;

    private final boolean isInitializer;

    LoxFunction(Stmt.Function declaration, Environment closure,
                boolean isInitializer) {
        this.isInitializer = isInitializer;
        this.closure = closure;
        this.declaration = declaration;
    }

    @Override
    public int arity() {
        return declaration.params.size();
    }

    @Override
    public String toString() {
        return "<fn " + declaration.name.lexeme + ">";
    }

    @Override
    public Object call(Interpreter interpreter,
                       List<Object> arguments) {
        Environment environment = new Environment(closure);
        for (int i = 0; i < declaration.params.size(); i++) {
            environment.define(declaration.params.get(i).lexeme,
                    arguments.get(i));
        }

        try {
            interpreter.executeBlock(declaration.body, environment);
        } catch (Return returnValue) {
            if (isInitializer) return closure.getAt(0, "this");
            return returnValue.value;
        }
        if (isInitializer) return closure.getAt(0, "this");
        return null;
    }

    // Chapter 13 Problem 2
    // Bind now takes second argument and defines inner. If no method, do nothing.
    LoxFunction bind(LoxInstance instance, LoxFunction inner) {
        Environment environment = new Environment(closure);
        environment.define("this", instance);
        environment.define("inner", inner != null ? inner : noInner());
        return new LoxFunction(declaration, environment,
                isInitializer);
    }

    // Chapter 13 Problem 2
    // Callable for no subclass method to do nothing
    private LoxCallable noInner() {
        final int count = declaration.params.size();
        return new LoxCallable() {
            @Override
            public int arity() {
                return count;
            }

            @Override
            public String toString() {
                return "No inner.";
            }

            @Override
            public Object call(Interpreter interpreter, List<Object> arguments) {
                return null;
            }

        };
    }
}