function encode(inputFile::IOStream, OutputFileName::IOStream)::Bool
    probabilityTable::Vector{Float32} = [1/256 for i in 1:256]
    frequencies::Vector{UInt64} = [] 
