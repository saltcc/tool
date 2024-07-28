do
    local version_str = string.match(_VERSION, "%d+[.]%d*")
    local version_num = version_str and tonumber(version_str) or 5.1
    local bit = (version_num >= 5.2) and require("bit32") or require("bit")

    local f_amr = Field.new("amr")
    local f_rtp = Field.new("rtp")
    local f_rtp_seq = Field.new("rtp.seq")
    local f_rtp_timestamp = Field.new("rtp.timestamp")
    local amrOctPacketLen = {14, 15, 17, 19, 21, 22, 28, 33, 0, 0, 0, 0, 0, 0, 0, 0}
    local amrwbOctPacketLen = {19, 25, 34, 38, 42, 48, 52, 60, 62, 0, 0, 0, 0, 0, 0, 0}


    local function export_amr_to_file()
        local tw = TextWindow.new("Export amr to File Info Win")
        local my_amr_tap = Listener.new(tap, "amr")
        local stream_infos = nil
        local octModle = true

        function twappend(str)
            tw:append(str)
            tw:append("\n")
        end

        function get_stream_info(pinfo)
            local key = "from_" .. tostring(pinfo.src) .. "_" .. tostring(pinfo.src_port) .. "to" .. tostring(pinfo.dst) .. "_" .. tostring(pinfo.dst_port)
            key = key:gsub(":", ".")
            local stream_info = stream_infos[key]
            if not stream_info then -- if not exists, create one
                stream_info = { }
                stream_info.filename = key.. ".amr"
                stream_info.file = io.open(stream_info.filename, "wb")
                stream_info.count = 0
                stream_info.iserror = false
                stream_infos[key] = stream_info
                twappend("Ready to export amr data (RTP from " .. tostring(pinfo.src) .. ":" .. tostring(pinfo.src_port) .. " to " .. tostring(pinfo.dst) .. ":" .. tostring(pinfo.dst_port) .. " to file:\n[" .. stream_info.filename .. "] ...\n")
            end

            return stream_info
        end

        local function writeOnebyte(f,x)
            local b=string.char(x%256)
            f:write(b)
        end

        local function write_packet_to_file(stream_info, str_bytes)
            stream_info.file:write(string.sub(str_bytes, 2))
        end

        local function amrBandeffit2Oct(amrStrData)
            return amrStrData
        end

        function my_amr_tap.packet(pinfo, tvb)
            local amr = f_amr()
            local rtpData = f_rtp()
            local rtp_seqsData = f_rtp_seq()
            local rtp_timestampsData = f_rtp_timestamp()

            local amrData = (version_num >= 5.2) and amr.range:bytes() or amr.value
            local amrStrData = (version_num >= 5.2) and amrData:tvb():raw() or amrData:tvb()():string()
            if octMode == false then
                amrStrData = amrBandeffit2Oct(amrStrData)
            end
                local stream_info = get_stream_info(pinfo)
            if stream_info.count == 0 then
                FT = bit.rshift(bit.band(string.byte(amrStrData, 2), 0x78), 3)
                --twappend(string.format("FT: %d\n", FT))
                if string.len(amrStrData) == amrOctPacketLen[FT+1] then
                    stream_info.file:write("#!AMR\n")
                elseif string.len(amrStrData) == amrwbOctPacketLen[FT+1] then
                    stream_info.file:write("#!AMR-WB\n")
                else
                    twappend(stream_info.filename .. "error or may not in octModle")
                    stream_info.iserror = true
                end
            end

            stream_info.count = stream_info.count + 1
            if stream_info.iserror == false then
                write_packet_to_file(stream_info, amrStrData)
            end
        end

        local function close_all_files()
            if stream_infos then
                local no_streams = true
                for id,stream in pairs(stream_infos) do
                    stream.file:flush()
                    stream.file:close()
                    twappend("File [" .. stream.filename .. "] generated OK!\n")
                    stream.file = nil
                    no_streams = false
                end
                if no_streams then
                    twappend("Not found any amr over rtp streams")
                end
            end
        end

        local function export_amr_oct()
            stream_infos = {}
            retap_packets()
            close_all_files()
            stream_infos = nil
        end

        local function export_amr_bandEffit()
            octMode = false
            stream_infos = {}
            retap_packets()
            close_all_files()
            stream_infos = nil
        end

        tw:add_button("[octMode] export to file", export_amr_oct)
        tw:add_button("[bandEffit] export to file", export_amr_bandEffit)
    end

    register_menu("Export amr to file", export_amr_to_file, MENU_TOOLS_UNSORTED)
end