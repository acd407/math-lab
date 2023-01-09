enum scale {
    Rest = 0 , 
    A0  = 21 , A0s = 22 , B0  = 23 , 
	C1  = 24 , C1s = 25 , D1  = 26 , D1s = 27 , E1  = 28 , F1  = 29 , F1s = 30 , G1  = 31 , G1s = 32 , 
	A1  = 33 , A1s = 34 , B1  = 35 , 
	C2  = 36 , C2s = 37 , D2  = 38 , D2s = 39 , E2  = 40 , F2  = 41 , F2s = 42 , G2  = 43 , G2s = 44 , 
	A2  = 45 , A2s = 46 , B2  = 47 , 
	C3  = 48 , C3s = 49 , D3  = 50 , D3s = 51 , E3  = 52 , F3  = 53 , F3s = 54 , G3  = 55 , G3s = 56 , 
	A3  = 57 , A3s = 58 , B3  = 59 , 
	C4  = 60 , C4s = 61 , D4  = 62 , D4s = 63 , E4  = 64 , F4  = 65 , F4s = 66 , G4  = 67 , G4s = 68 , 
	A4  = 69 , A4s = 70 , B4  = 71 , 
	C5  = 72 , C5s = 73 , D5  = 74 , D5s = 75 , E5  = 76 , F5  = 77 , F5s = 78 , G5  = 79 , G5s = 80 , 
	A5  = 81 , A5s = 82 , B5  = 83 , 
	C6  = 84 , C6s = 85 , D6  = 86 , D6s = 87 , E6  = 88 , F6  = 89 , F6s = 90 , G6  = 91 , G6s = 92 , 
	A6  = 93 , A6s = 94 , B6  = 95 , 
	C7  = 96 , C7s = 97 , D7  = 98 , D7s = 99 , E7  = 100, F7  = 101, F7s = 102, G7  = 103, G7s = 104, 
	A7  = 105, A7s = 106, B7  = 107, 
	C8  = 108, 
};

enum voice {
    L1 = C3, L2 = D3, L3 = E3, L4 = F3, L5 = G3, L6 = A3, L7 = B3,
    M1 = C4, M2 = D4, M3 = E4, M4 = F4, M5 = G4, M6 = A4, M7 = B4,
    H1 = C5, H2 = D5, H3 = E5, H4 = F5, H5 = G5, H6 = A5, H7 = B5,
	SPEED_INTERVAL = 200, HIGH_SPEED = 300, 
	MIDDLE_SPEED = HIGH_SPEED + SPEED_INTERVAL, 
    LOW_SPEED = MIDDLE_SPEED + SPEED_INTERVAL, 
    HALF_SPEED = 0xef1, DOUBLE_SPEED = 0xef2, 
    _2 = 2, _4 = 4, _8 = 8, 
    __ = 0xff
};