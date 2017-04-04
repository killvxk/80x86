template <typename T>
std::pair<uint16_t, T> do_ror(T v, int count)
{
    uint16_t flags = 0;

    for (int i = 0; i < count; ++i) {
        flags &= ~CF;
        if (v & 0x1)
            flags |= CF;
        T lsb = v & 1;
        v >>= 1;
        v += (lsb << ((sizeof(T) * 8) - 1));
    }

    // 2 MSB's XOR'd == OF
    T msbs = (v >> ((sizeof(T) * 8) - 2)) & 0x3;
    if (msbs == 2 || msbs == 1)
        flags |= OF;

    return std::make_pair(flags, v);
}

// ror r/m, 1
void EmulatorPimpl::rord0()
{
    auto v = read_data<uint8_t>();
    uint16_t flags;

    std::tie(flags, v) = do_ror(v, 1);

    write_data<uint8_t>(v);
    registers->set_flags(flags, CF | OF);
}

// ror r/m, 1
void EmulatorPimpl::rord1()
{
    auto v = read_data<uint16_t>();
    uint16_t flags;

    std::tie(flags, v) = do_ror(v, 1);

    write_data<uint16_t>(v);
    registers->set_flags(flags, CF | OF);
}

// ror r/m, N
void EmulatorPimpl::rord2()
{
    auto v = read_data<uint8_t>();
    uint16_t flags;

    if (!registers->get(CL))
        return;

    std::tie(flags, v) = do_ror(v, registers->get(CL));

    write_data<uint8_t>(v);
    registers->set_flags(flags, CF);
}

// ror r/m, N
void EmulatorPimpl::rord3()
{
    auto v = read_data<uint16_t>();
    uint16_t flags;

    if (!registers->get(CL))
        return;

    std::tie(flags, v) = do_ror(v, registers->get(CL));

    write_data<uint16_t>(v);
    registers->set_flags(flags, CF);
}