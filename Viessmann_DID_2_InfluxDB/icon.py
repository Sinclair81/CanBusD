# Let's generate a valid PNG image array with 48x48 resolution to keep it compact.
# The image should be fully Viessmann red (#cc0000) with a white stylized wavy heat symbol (♨️) in the center, without any white square/box.

import cv2
import numpy as np

# Create a 48x48 image with Viessmann Red (BGR format: #0000cc)
img = np.zeros((48, 48, 3), dtype=np.uint8)
img[:] = [0, 0, 0xcc]

# Let's draw 3 wavy curves resembling the ♨ icon using a white color (255, 255, 255)
# Wave 1 (Left)
pts1 = np.array([[16, 36], [12, 28], [16, 20], [12, 12]], dtype=np.int32)
cv2.polylines(img, [pts1], False, (255, 255, 255), 2, cv2.LINE_AA)

# Wave 2 (Middle)
pts2 = np.array([[24, 36], [20, 28], [24, 20], [20, 12]], dtype=np.int32)
cv2.polylines(img, [pts2], False, (255, 255, 255), 2, cv2.LINE_AA)

# Wave 3 (Right)
pts3 = np.array([[32, 36], [28, 28], [32, 20], [28, 12]], dtype=np.int32)
cv2.polylines(img, [pts3], False, (255, 255, 255), 2, cv2.LINE_AA)

# Draw the bottom plates/lines of the ♨ symbol
cv2.line(img, (12, 39), (36, 39), (255, 255, 255), 2, cv2.LINE_AA)
cv2.line(img, (16, 42), (32, 42), (255, 255, 255), 2, cv2.LINE_AA)

# Encode to PNG
success, encoded_img = cv2.imencode('.png', img)
bytes_data = encoded_img.tobytes()

# Format as C++ array with 12 bytes per line
hex_lines = []
for i in range(0, len(bytes_data), 12):
    chunk = bytes_data[i:i+12]
    hex_line = ", ".join([f"0x{b:02X}" for b in chunk])
    hex_lines.append("    " + hex_line + ",")

print(f"Length: {len(bytes_data)}")
print("\n".join(hex_lines))
