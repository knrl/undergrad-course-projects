library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

-- Asagidaki kütüphaneler txt dosya okumak için gerekli
library STD;
use STD.TEXTIO.ALL;

-- Görseli depolamak için olu?uturulan RAM entity'si
entity im_ram is
	generic(
		ADDR_WIDTH : integer := 16;	-- Image Ram'in adres bus'inin genisligi (boyutu)
		IM_SIZE_D1 : integer := 64;	-- Görselin satir sayisi
		IM_SIZE_D2 : integer := 64	-- Görselin sutun sayisi
	);

	port (
		Clk        : in std_logic;
		addr_in    : in unsigned(ADDR_WIDTH-1 downto 0);  -- Image RAM'in adres bus'i
		rgb_out    : out std_logic_vector(23 downto 0)	  -- 24 bit RGB piksel cikisi temsil ediyor
	);
end im_ram;

architecture Behav of im_ram is

-- Görsel için olusturulan custom array tanimlamasi
type im_ram_type is array (0 to  IM_SIZE_D1*IM_SIZE_D2-1) of std_logic_vector(23 downto 0); -- 64*64,24 bitlik array'i temsil eder.

-- txt dosyas?ndan görselin piksellerini okuyan ve RAM'in ilk de?erini atayan fonksiyon 
impure function im_ram_initialize return im_ram_type is
	variable line_var     : line;
	file text_var         : text;
	variable pixel        : integer;
	variable image_pixels : im_ram_type;
begin
	file_open(text_var,"C:\Users\kaane\embedded_proje\rgb.txt",read_mode);	    -- Dosyayi okuma modunda aç
	while(NOT ENDFILE(text_var)) loop 	            -- Dosya sonuna kadar dön
		for k in 1 to 3 loop	                    -- k degiskeni R,G,B'yi temsilen 3 deger al?r
			for j in 0 to IM_SIZE_D2-1 loop         -- Satir sayisi kadar dön
				for i in 0 to IM_SIZE_D1-1 loop     -- Sutun sayisi kadar dön
					readline(text_var,line_var);    -- Bir satir okur, her satir 1 piksel içerir.
					read(line_var,pixel);	        -- line_var içerisinden integer degeri okur. Cünkü line_var = "123\n" seklindedir.
					image_pixels(i*IM_SIZE_D2+j)(k*8-1 downto k*8-8) := std_logic_vector(to_unsigned(pixel,8));    -- Piksel degerini RAM'de depola.
				end loop;
			end loop;
		end loop;
	end loop;
	file_close(text_var);                           -- Dosya göstericisini kapat.
	return image_pixels;	
end function;

signal ram : im_ram_type := im_ram_initialize;      -- im_ram_type tipinde bir RAM tanimla ve ilk degerini ata

begin
                                                    -- Adres Bus -(in)-> RAM -(out)-> RGB Out
rgb_out <= ram(to_integer(addr_in));                -- Adres bus'in girisi ile RAM'den R,G,B pikselleri al.
                                                    -- addr_in, 16 bit ve tam sayi karsiligina denk gelen indisdeki
                                                    -- RAM'de bulunan 24 bitlik piksel deger alinir.
end architecture;