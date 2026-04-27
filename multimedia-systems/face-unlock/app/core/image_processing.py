import cv2
import face_recognition
import numpy as np

from typing import BinaryIO, Tuple, List, Dict
from typing_extensions import Self


class ImageProcessing:
    def __init__(self: Self, model: str) -> Self:
        self.model = model

    def authenticate(
        self: Self, buffer: BinaryIO, face_encodings: Dict
    ) -> Tuple[any, str or None]:

        # se non c'è nessun encoding memorizzaato, nessun utente è autorizzato
        if len(face_encodings["encodings"]) == 0:
            return None, None

        img = self.from_buffer(buffer)
        if img is None:
            raise Exception("cannot read image")

        unknown_face_locations, unknown_face_encodings = self.__analyze_image__(img)

        if len(unknown_face_encodings) == 0:
            return img, []

        face_users = []
        for unknown_face_encoding in unknown_face_encodings:
            matches = face_recognition.compare_faces(
                face_encodings["encodings"], unknown_face_encoding
            )

            user = None

            # troviamo le distanze tra gli encoding
            face_distances = face_recognition.face_distance(
                face_encodings["encodings"], unknown_face_encoding
            )

            # troviamo il minimo tra le distanze calcolate sopra
            best_match_index = np.argmin(face_distances)
            if matches[best_match_index]:
                user = face_encodings["users"][best_match_index]

            # aggiungiamo il nome alla lista
            face_users.append(user)

        # disegnamo i rettangoli sui volti con i rispettivi nomi
        self.__draw_on_faces__(
            img,
            unknown_face_locations,
            [user.username for user in face_users if user is not None],
        )

        return img, face_users

    def register(self: Self, username: str, buffers: List[BinaryIO]):
        res = []
        encodings = []

        for buffer in buffers:
            img = self.from_buffer(buffer)

            if img is None:
                raise Exception("cannot read image")

            locations, new_encodings = self.__analyze_image__(img)

            # concateno la lista di encodings
            encodings += new_encodings

            # disegnamo i rettangoli sui volti con i rispettivi nomi
            self.__draw_on_faces__(img, locations, [username for _ in new_encodings])

            # codifichiamo l'immagine in jpeg
            res.append(self.to_jpeg(img))

        return res, encodings

    def __analyze_image__(self: Self, img):
        rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

        # calolo del riconoscimento dei volti
        face_locations = face_recognition.face_locations(rgb, model=self.model)

        # calcolo degli encodings dei volti riconosciuti
        face_encodings = face_recognition.face_encodings(rgb, face_locations)

        return face_locations, face_encodings

    def __draw_on_faces__(
        self: Self,
        img,
        face_locations: List,
        face_names: List[str],
    ) -> any:
        for (top, right, bottom, left), name in zip(face_locations, face_names):
            cv2.rectangle(img, (left, top), (right, bottom), (0, 0, 255), 2)
            cv2.rectangle(
                img, (left, bottom - 35), (right, bottom), (0, 0, 255), cv2.FILLED
            )
            font = cv2.FONT_HERSHEY_DUPLEX
            cv2.putText(
                img, name, (left + 6, bottom - 6), font, 1.0, (255, 255, 255), 1
            )
        return img

    def from_buffer(self: Self, buffer: BinaryIO):
        return cv2.imdecode(np.frombuffer(buffer.read(), np.uint8), cv2.IMREAD_COLOR)

    def to_jpeg(self: Self, img):
        """
        codifico l'immagine opencv in jpeg
        """
        ret, enc = cv2.imencode(".jpeg", img)
        if not ret:
            raise Exception("cannot encode image")
        return enc

    def to_file(self: Self, filename: str, img):
        return cv2.imwrite(filename, img)
