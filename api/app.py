# app.py
from fastapi import FastAPI, Query
from kdtree_wrapper import lib, Tarv, TReg
from ctypes import POINTER,c_char
from pydantic import BaseModel

app = FastAPI()


class PontoEntrada(BaseModel):
    lat: float
    lon: float
    nome: str

@app.post("/construir-arvore")
def constroi_arvore():
    lib.kdtree_construir()
    return {"mensagem": "Árvore KD inicializada com sucesso."}

@app.post("/inserir")
def inserir(ponto: PontoEntrada):
    nome_bytes = ponto.nome.encode('utf-8')[:99]  # Trunca se necessário
    novo_ponto = TReg(lat=ponto.lat, lon=ponto.lon, nome=nome_bytes)
    lib.inserir_ponto(novo_ponto)
    return {"mensagem": f"Ponto '{ponto.nome}' inserido com sucesso."}

@app.get("/buscar")
def buscar(lat: float = Query(...), lon: float = Query(...)):
    query = TReg(lat=lat, lon=lon)

    arv = lib.get_tree()  # Suponha que esta função retorne ponteiro para árvore já construída
    resultado = lib.buscar_mais_proximo(arv, query)

    return {
        "lat": resultado.lat,
        "lon": resultado.lon,
        "nome": resultado.nome
    }

