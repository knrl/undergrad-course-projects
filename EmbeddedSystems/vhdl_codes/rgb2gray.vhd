library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

-- RAM'de tutulan RGB görselden gri görsele çeviren kod
entity rgb2gray is
	generic(
		ADDR_WIDTH : integer := 16;	    -- RAM'in adres bus'inin genisligi (boyutu)
		IM_SIZE_D1 : integer := 64;	    -- Görselin satir sayisi
		IM_SIZE_D2 : integer := 64	    -- Görselin sütun sayisi
	);

	port (
		Clk        : in std_logic;
		reset      : in std_logic;	            -- asenkron reset için
		data_valid : out  std_logic;	        -- gray_out'un uygun ciktisi varsa 1 olur
		gray_out   : out unsigned(7 downto 0)	-- 8 bitlik gri piksel ciktisi
	);
end rgb2gray;

architecture Behav of rgb2gray is
	component im_ram is                 -- im_ram'den bir RAM bilesenin tanimlanmasi
		generic(
			ADDR_WIDTH : integer := 16;	-- RAM_ENTITY: RAM'in adres bus'inin genisligi (boyutu)
			IM_SIZE_D1 : integer := 64;	-- RAM_ENTITY: Görselin satir sayisi
			IM_SIZE_D2 : integer := 64	-- RAM_ENTITY: Görselin sutun sayisi
		);

		port (
			Clk        : in std_logic;
			addr_in    : in unsigned(ADDR_WIDTH-1 downto 0); -- RAM_ENTITY: Image RAM'in adres bus'i
			rgb_out    : out std_logic_vector(23 downto 0)	 -- RAM_ENTITY: 24 bit RGB piksel ciktisi
		);
	end component;

	signal rgb_out : std_logic_vector(23 downto 0);          -- RAM'den alinan RGB pikseli (24 bit)
	signal addr_in : unsigned(ADDR_WIDTH-1 downto 0);        -- RAM'den ilgili indisi almak için verilecek degeri temsil eder (adresi ileten bus)

begin

	-- RAM'in tanimlanmasi
	image_ram : im_ram generic map(ADDR_WIDTH,	IM_SIZE_D1 ,IM_SIZE_D2)
		port map(Clk, addr_in, rgb_out);

	-- RGB'den griye çeviren process
	CONVERTER_PROC : process(Clk,reset)
		variable temp1,temp2,temp3,temp4 : unsigned(15 downto 0);  -- geçici degiskenler
	begin
		if(reset = '1') then	                                   -- asenkron reset aktif ise
			addr_in <= (others => '0');                            -- adres bus'i sifirla
			data_valid <= '0';                                     -- uygun data temsilini sifirla
		elsif rising_edge(Clk) then
			if(to_integer(addr_in) = IM_SIZE_D1*IM_SIZE_D2-1) then -- RAM'deki son adrese ulasildiysa aktif olur
				addr_in <= (others => '0');
				data_valid <= '0';
			else	                                               -- RAM'in sonuna ulasilmadiysa arttirmaya devam et
				addr_in <= addr_in + 1;
				data_valid <= '1';	                               -- çiktinin hazir oldugunu temsil eder
			end if;
			-- Gri piksel = 0.3*Kirmizi piksel + 0.59*Yesil piksel + 0.11*Mavi piksel
			temp1 := "01001100" * unsigned(rgb_out(7 downto 0));   -- (0.3 * R)  
			temp2 := "10010111" * unsigned(rgb_out(15 downto 8));  -- (0.59 * G) 
			temp3 := "00011100" * unsigned(rgb_out(23 downto 16)); -- (0.11 * B)
			temp4 := temp1 + temp2 + temp3;
			gray_out <= temp4(15 downto 8) + ("0000000" & temp4(7)); -- Sonucu yuvarlar
		end if;
	end process;

end architecture;