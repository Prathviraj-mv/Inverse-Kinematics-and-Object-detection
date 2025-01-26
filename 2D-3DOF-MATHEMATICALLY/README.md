Description                           | Formula
--------------------------------------|----------------------------------------------------------------------------------------
Effective radius (r_eff)              | r_eff = sqrt(target_x^2 + target_z^2) - L3
Effective z-position (z_eff)          | z_eff = target_z
Reachability check                    | r_eff < 0 or sqrt(r_eff^2 + z_eff^2) > L1 + L2
Elbow angle (theta2)                  | cos(theta2) = (r_eff^2 + z_eff^2 - L1^2 - L2^2) / (2 * L1 * L2)
Elbow angle (theta2)                  | theta2 = acos(cos(theta2))
Shoulder angle (theta1)               | theta1 = atan2(z_eff, r_eff) - atan2(L2 * sin(theta2), L1 + L2 * cos(theta2))
Wrist angle (theta3)                  | theta3 = desired_orientation - (theta1 + theta2)
                                      | theta1_deg = degrees(theta1)
                                      | theta2_deg = degrees(theta2)
                                      | theta3_deg = degrees(theta3)



![Screenshot 2025-01-26 121128](https://github.com/user-attachments/assets/50126028-34d5-4f9a-a2b8-26dcccad40e9)
