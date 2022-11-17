# Composicion, utilizar ∘ en lugar de '.' (Haskell)
# Nombres de variables siguen criterio utilizado en catedra (recordar tiempos de examen)

mutable struct Resultados
    PPS::Float64
    PEC::Float64
    PTO::Float64
    PARRC::Float64
end

HV = Inf64

#######################################################

#Utils para currying (permite aplicacion parcial mas estilo Haskell)
#Utils para generacion de valores aleatorios para las fdp dadas

curry(f, x) = (xs...) -> f(x, xs...)

aplanarFloatEntero = curry(floor,Int64)
inversaDistribucionUniforme(supremo,infimo,x) = infimo + (supremo - infimo)*x
inversaTA(x) = √(300*x + 100)

function obtenerIA()
    unValor = (aplanarFloatEntero ∘ inversaDistribucionUniforme)(10,0,rand())
    return unValor
end

function obtenerTA()
    unValor = (aplanarFloatEntero ∘ inversaTA ∘ rand)()
    return unValor
end

#######################################################

function main()
    T = 0
    NS = 0
    NT = 0
    TPLL = 0
    TPS = HV
    STS = 0
    STO = 0
    STA = 0
    ITO = 0
    ARR = 0
    ARRC = 0
    TF = 94784569
    sq = true
    unosResultados = Resultados(0,0,0,0)
    println("-------------- SIMULACION UNA COLA / UN PUESTO DE ATENCION --------------")

    while(true)
        if TPLL <= TPS
            STS += (TPLL-T)*NS
            T = TPLL
            IA = obtenerIA()
            TPLL = T + IA
            sq,ARR,ARRC = arrepentimiento(NS,ARR,ARRC)
            if(sq)
                NS+=1
                NT+=1
                if(NS == 1)
                    STO += T - ITO
                    TA = obtenerTA()
                    TPS = T + TA
                    STA += TA
                end
            end
        else
            STS += (TPS - T)*NS
            T = TPS
            NS-=1
            if(NS >= 1)
                TA = obtenerTA()
                TPS = T + TA
                STA += TA
            else
                ITO = T
                TPS = HV
            end
        end

        if(T > TF)
            unosResultados.PPS = STS/NT
            unosResultados.PEC = (STS - STA)/NT
            unosResultados.PTO = (STO * 100)/T
            unosResultados.PARRC = (ARRC * 100)/ARR
            println(unosResultados)
            break
        end
    end
end

function arrepentimiento(NS,ARR,ARRC)
    sq = false
    r = 0
    personasEnCola = NS - 1

    if(personasEnCola <=4)
        sq = true
    elseif(personasEnCola > 8)
        sq = false
    else
        r = rand()
        if(r<0.4)
            sq = true
        else
            sq = false
            ARR+=1
            if(personasEnCola == 5)
                ARRC +=1
            end
        end
    end

    return sq, ARR, ARRC
end

if abspath(PROGRAM_FILE) == @__FILE__
    main()
end

#######################################################
