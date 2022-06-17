library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

library STD;
use STD.TEXTIO.ALL;

entity tb is
end tb;

architecture sim of tb is

	-- Generic parametreler sabit olarak tanimlandi ve ilk degerleri atandi
	constant IM_SIZE_D1: integer := 64;
	constant IM_SIZE_D2: integer := 64;
	constant ADDR_WIDTH: integer := 12;	-- 64*64=4096 oldugundan 12 bit gerekli

	-- gri piksel ciktilarini depolamak icin
	type image_type is array (1 to  IM_SIZE_D1, 1 to  IM_SIZE_D2) of integer;
	signal image_pixels : image_type := (others => (others => 0));

	-- bilesen tanimlamasi
	component rgb2gray is
		generic(
			ADDR_WIDTH : integer := 16;
			IM_SIZE_D1 : integer := 64;
			IM_SIZE_D2 : integer := 64
		);

		port (
			Clk        : in std_logic;
			reset      : in std_logic;
			data_valid : out  std_logic;
			gray_out   : out unsigned(7 downto 0)
		);
	end component;

	-- sinyallerin tanimlanmasi
	signal Clk,reset,data_valid,check_end_of_file : std_logic := '0';
	signal gray_out     : unsigned(7 downto 0);
	constant Clk_period : time := 10 ns;	-- clock periyodu

begin

	-- clock sinyal üret
	Clk <= not Clk after Clk_period/2;
	
	UUT : rgb2gray generic map(ADDR_WIDTH, IM_SIZE_D1, IM_SIZE_D2)
			port map(Clk, reset, data_valid, gray_out);

	-- Girdilerin uygulandigi, ciktilarin okundugu ve sonuclarin degerlendirildigi process
	STIMULUS_PROC : process
		variable i,j,k,l  : integer := 1;	-- dongu indisi
		variable line_var : line;
		file text_var     : text;           -- metin dosyasi olarak tanimlandi
	begin
		reset <= '1';
		wait for Clk_period;
		reset <= '0';	                                -- ilk clock'da reset uygulandi
		wait until data_valid = '1';	                -- cikti portundan uygun data'nin gelmesi bekle
		while(data_valid = '1') loop
			wait until (falling_edge(Clk));             -- düsen clock kenarinda tetiklenir
			image_pixels(i,j) <= to_integer(gray_out);	-- pikseli tam sayi olarak kaydet
			if(j = IM_SIZE_D2) then                     -- indisleri uret (satir ve sutun olarak)
				j := 1;
				if(i = IM_SIZE_D1) then
					i := 1;
				else
					i := i+1;
				end if;
			else
				j := j+1;
			end if;	
			wait until (rising_edge(Clk));	            -- yukselen clock kenarina kadar bekle            -- yanlizca dusen ve yukselen arasindaki arada calismasi icin senkronizasyon için
		end loop;                                       -- tüm piksel griye cevrildiyse reset'i aktif et
		reset <= '1';

		file_open(text_var,"C:\Users\kaane\embedded_proje\result_gray.txt",write_mode);
        for k in 1 to IM_SIZE_D2 loop
            for l in 1 to IM_SIZE_D1 loop
                write(line_var,integer'image(image_pixels(l,k)));
                writeline(text_var,line_var);
                wait for 1 ns;	                        -- her 1 pikselin hesaplanmasindan sonra 1 ns bekle 
            end loop;
        end loop;
		check_end_of_file <= '1';
		file_close(text_var);                           -- dosya gostericisini kapat

		wait;	                                        -- simulasyon bitine kadar bekle
	end process;

end architecture;