declare class File
    fd:number
end

declare io: {
    close: ((file: File) -> number? ),
    flush: (() -> nil ),
    input: ((file: string | File) -> File? ),
    lines: ((file: string | File) -> string? ),
}