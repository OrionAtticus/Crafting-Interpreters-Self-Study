package com.craftinginterpreters.lox;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

class LoxClass implements LoxCallable {
    final String name;
    final LoxClass superclass;

    private final Map<String, LoxFunction> methods;

    LoxClass(String name, LoxClass superclass,
             Map<String, LoxFunction> methods) {
        this.superclass = superclass;
        this.name = name;
        this.methods = methods;
    }

    @Override
    public String toString() {
        return name;
    }

    @Override
    public Object call(Interpreter interpreter,
                       List<Object> arguments) {
        LoxInstance instance = new LoxInstance(this);
        // Chapter 13 Problem 2
        LoxFunction initializer = findMethod(instance,"init");
        if (initializer != null) {
            initializer.call(interpreter, arguments);
        }
        return instance;
    }

    // Chapter 13 Problem 2
    @Override
    public int arity() {
        LoxFunction initializer = findMethod(null, "init");
        if (initializer == null) return 0;
        return initializer.arity();
    }

    // Chapter 13 Problem 2
    LoxFunction findMethod(LoxInstance instance, String name) {
        // Instead of doing the first match, Find every method with the same name, subclasses first
        List<LoxFunction> chain = new ArrayList<>();
        for (LoxClass klass = this;  klass != null; klass = klass.superclass) {
            LoxFunction method = klass.methods.get(name);
            if (method != null)
                chain.add(method);
        }
        // After collecting, bind from bottom up
        LoxFunction inner = null;
        for (LoxFunction method : chain) {
            inner = method.bind(instance, inner);
        }
        // Either top method or null if none
        return inner;
    }
}