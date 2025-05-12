import ctypes
from ctypes import Structure, POINTER, c_double, c_int,c_char

class TReg(Structure):
    _fields_ = [("lat", c_double), 
                ("lon", c_double),
                ("nome",c_char *100)
                ]

class TNode(Structure):
    pass

TNode._fields_ = [("key", ctypes.c_void_p),
                  ("esq", POINTER(TNode)),
                  ("dir", POINTER(TNode))]

class Tarv(Structure):
    _fields_ = [("k", c_int),
                ("dist", ctypes.CFUNCTYPE(c_double, ctypes.c_void_p, ctypes.c_void_p)),
                ("cmp", ctypes.CFUNCTYPE(c_int, ctypes.c_void_p, ctypes.c_void_p, c_int)),
                ("raiz", POINTER(TNode))]

# Carregar a biblioteca C
lib = ctypes.CDLL("./libkdtree.so")

# Definir a assinatura da função
lib.buscar_mais_proximo.argtypes = [POINTER(Tarv), TReg]
lib.buscar_mais_proximo.restype = TReg
lib.get_tree.restype = POINTER(Tarv)
lib.inserir_ponto.argtypes = [TReg]
lib.inserir_ponto.restype = None
lib.kdtree_construir.argtypes = []
lib.kdtree_construir.restype = None
