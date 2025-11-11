#!/usr/bin/env python3
from math import *
from os import system
import readline

SAFE_DICT = {name: obj for name, obj in globals().items() if name in (
    'acos', 'asin', 'atan', 'atan2', 'ceil', 'cos', 'cosh', 'degrees',
    'e', 'exp', 'fabs', 'factorial', 'floor', 'fmod', 'frexp', 'hypot',
    'ldexp', 'log', 'log10', 'modf', 'pi', 'pow', 'radians', 'sin', 'sinh',
    'sqrt', 'tan', 'tanh'
)}

while True:
    expr = input("[calculator] >>> ").strip()
    if expr.lower() == "exit":
        break
    if expr.lower() == "clear":
        system("clear")
        continue
    if not expr:
        continue
    try:
        result = eval(expr, {"__builtins__": None}, SAFE_DICT)
        print(result)
    except Exception as e:
        print("Error:", e)
